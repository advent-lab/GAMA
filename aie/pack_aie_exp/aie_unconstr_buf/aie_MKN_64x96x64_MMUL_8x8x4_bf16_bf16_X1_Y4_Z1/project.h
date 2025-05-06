
#include <vector>

#include "include.h"
#include "kernels.h"
#include <adf.h>

#define ADF_DEBUG

#ifdef ADF_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

using namespace adf;

class simpleGraph : public adf::graph {
private:
    kernel mat_mul_k[mult_X * mult_Y * mult_Z];

public:
    input_plio A[mult_X * mult_Y];
    input_plio B[mult_Y * mult_Z];
    output_plio C[mult_X * mult_Z];

    simpleGraph() {

        DEBUG(printf("Debug simpleGraph AIE placement and buffer allocation\n"));
        DEBUG(printf("Total AIEs: %d\n", mult_X * mult_Y * mult_Z));
        DEBUG(printf("Total PLIOs: %d (IN %d OUT %d)\n",
                     (mult_X * mult_Y + mult_Y * mult_Z + mult_X * mult_Z),
                     (mult_X * mult_Y + mult_Y * mult_Z), mult_X * mult_Z));
        // input and output plio creation below
        // A: 0   1    2  ...
        //    x  x+1  x+2 ...
        for (int i = 0; i < mult_X * mult_Y; i++) {
            A[i] = input_plio::create("A" + std::to_string(i), plio_128_bits,
                                      "./matA" + std::to_string(i) + ".txt");
            DEBUG(printf("PLIO A[%d] file: ./matA%d.txt\n", i, i));
        }

        // B: 0   y
        //    1  y+1
        //    2  y+2
        //   ... ...
        for (int i = 0; i < mult_Y * mult_Z; i++) {
            B[i] = input_plio::create("B" + std::to_string(i), plio_128_bits,
                                      "./matB" + std::to_string(i) + ".txt");
            DEBUG(printf("PLIO B[%d] file: ./matB%d.txt\n", i, i));
        }

        // C: 0   1    2  ...
        //    x  x+1  x+2 ...
        for (int i = 0; i < mult_X * mult_Z; i++) {
            C[i] = output_plio::create("C" + std::to_string(i), plio_128_bits,
                                       "./matC" + std::to_string(i) + ".txt");
            DEBUG(printf("PLIO C[%d] file: ./matC%d.txt\n", i, i));
        }

        int kernel_count = 0;
        // Kernel count for kernel assignment
        int k_count_ka = 0;
        int tile_x = 0;
        int tile_y = 0;
        int plio_A = 0;
        int plio_B = 0;
        int plio_C = 0;

        // mult_Z = 4
        // mult_X = 1

        // X dimension scales the AIEs over the rows
        for (int j = 0; j < mult_X; j++) {
            // Z dimension scales the AIEs over the column
            for (int k = 0; k < mult_Z; k++) {
                // Use Y axis for grouping the AIE's for reduction
                for (int i = 0; i < mult_Y; i++) {
                    k_count_ka = (j * mult_Z * mult_Y) + (k * mult_Y) + i;
                    DEBUG(printf("Kernel count for kernel assignment: %d\n", k_count_ka));
                    if (i == 0) {
                        mat_mul_k[k_count_ka] =
                            kernel::create(opt_blocked_matrix_mult_bf16A_bf16B_o32PS);
                    } else if (i == mult_Y - 1) {
                        mat_mul_k[k_count_ka] = kernel::create(
                            opt_blocked_matrix_mult_bf16A_bf16B_i32PS_o32C);
                    } else {
                        mat_mul_k[k_count_ka] = kernel::create(
                            opt_blocked_matrix_mult_bf16A_bf16B_i32PS_o32PS);
                    }
                }

                // Kernel creation below
                for (int i = 0; i < mult_Y; i++) {
                    tile_x = (k * mult_Y) + i;
                    if (j % 2) {
                        tile_x = (k * mult_Y) + i + 2;
                    } else {
                        tile_x = (k * mult_Y) + i;
                    }
                    tile_y = j;
                    // kernel_count = i;
                    kernel_count = (j * mult_Z * mult_Y) + (k * mult_Y) + i;
                    plio_A = (j * mult_Y) + i;
                    plio_B = (k * mult_Y) + i;
                    // plio_C = i % (mult_Y - 1); // One PLIO out for each group
                    plio_C = j * mult_Z + k; // One PLIO out for each group
                    DEBUG(printf("AIE_Tile(%d,%d)\n", tile_x, tile_y));
                    DEBUG(printf("Kernel %d\n", kernel_count));
                    DEBUG(printf("PLIO(A[%d],B[%d])\n", plio_A, plio_B));

                    // Kernel 0 should be opt_blocked_matrix_mult_i8A_i8B_o32PS
                    // First AIE from the group
                    if (i == 0) {
                        // aie = (i * Y + j, k)
                        // Kernel number

                        connect<>(A[plio_A].out[0], mat_mul_k[kernel_count].in[0]);
                        dimensions(mat_mul_k[kernel_count].in[0]) = {single_M * single_K *
                                                                     1};

                        DEBUG(printf(
                            "PL-Kernel: A[%d] to kernel_%d_in0 with dimension %d \n",
                            plio_A, kernel_count, single_M * single_K));
                        connect<>(B[plio_B].out[0], mat_mul_k[kernel_count].in[1]);
                        dimensions(mat_mul_k[kernel_count].in[1]) = {single_K * single_N *
                                                                     1};

                        DEBUG(printf(
                            "PL-Kernel: B[%d] to kernel_%d_in1 with dimension %d \n",
                            plio_B, kernel_count, single_K * single_N));

                        connect<cascade>(mat_mul_k[kernel_count].out[0],
                                         mat_mul_k[kernel_count + 1].in[2]);

                        DEBUG(printf("Cascade: kernel_%d_out0 to kernel_%d_in2 \n",
                                     kernel_count, kernel_count + 1));
                        source(mat_mul_k[kernel_count]) =
                            "kernels/kernels_bf16A_bf16B_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        // location<stack>(mat_mul_k[kernel_count]) = {
                        //     address(tile_x, tile_y, 0x0000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                        //     address(tile_x, tile_y, 0x1500),
                        //     address(tile_x, tile_y, 0x4000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                        //     address(tile_x, tile_y, 0x8000),
                        //     address(tile_x, tile_y, 0xC000)};
                    }
                    // Last kernel in the group should have outputs to PLIO
                    // Last AIE from the group
                    else if (i == mult_Y - 1) {

                        connect<>(A[plio_A].out[0], mat_mul_k[kernel_count].in[0]);
                        dimensions(mat_mul_k[kernel_count].in[0]) = {single_M * single_K *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: A[%d] to kernel_%d_in0 with dimension %d \n",
                            plio_A, kernel_count, single_M * single_K));

                        connect<>(B[plio_B].out[0], mat_mul_k[kernel_count].in[1]);
                        dimensions(mat_mul_k[kernel_count].in[1]) = {single_K * single_N *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: B[%d] to kernel_%d_in1 with dimension %d \n",
                            plio_B, kernel_count, single_K * single_N));

                        connect<>(mat_mul_k[kernel_count].out[0], C[plio_C].in[0]);
                        dimensions(mat_mul_k[kernel_count].out[0]) = {single_M *
                                                                      single_N};
                        DEBUG(printf(
                            "Kernel-Pl: C[%d] to kernel_%d_out0 with dimension %d \n",
                            plio_C, kernel_count, single_M * single_N));

                        source(mat_mul_k[kernel_count]) =
                            "kernels/kernels_bf16A_bf16B_i32PS_o32C.cc";

                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 1 buffer locations
                        // location<stack>(mat_mul_k[kernel_count]) = {
                        //     address(tile_x, tile_y, 0xE000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                        //     address(tile_x, tile_y, 0x0),
                        //     address(tile_x, tile_y, 0x4000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                        //     address(tile_x, tile_y, 0x8000),
                        //     address(tile_x, tile_y, 0xC000)};
                        // location<buffer>(mat_mul_k[kernel_count].out[0]) = {
                        //     address(tile_x - 1, tile_y, 0x2000),
                        //     address(tile_x - 1, tile_y, 0xA000)};
                    }
                    // Kernel in between should have cascade input and output for
                    // Second last AIE from the group
                    // partial
                    // sum
                    else if (i == mult_Y - 2) {

                        connect<>(A[plio_A].out[0], mat_mul_k[kernel_count].in[0]);
                        dimensions(mat_mul_k[kernel_count].in[0]) = {single_M * single_K *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: A[%d] to kernel_%d_in0 with dimension %d \n",
                            plio_A, kernel_count, single_M * single_K));

                        connect<>(B[plio_B].out[0], mat_mul_k[kernel_count].in[1]);
                        dimensions(mat_mul_k[kernel_count].in[1]) = {single_K * single_N *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: B[%d] to kernel_%d_in1 with dimension %d \n",
                            plio_B, kernel_count, single_K * single_N));

                        connect<cascade>(mat_mul_k[kernel_count].out[0],
                                         mat_mul_k[kernel_count + 1].in[2]);
                        DEBUG(printf("Cascade: kernel_%d_out0 to kernel_%d_in2 \n",
                                     kernel_count, kernel_count + 1));

                        source(mat_mul_k[kernel_count]) =
                            "kernels/kernels_bf16A_bf16B_i32PS_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        // location<stack>(mat_mul_k[kernel_count]) = {
                        //     address(tile_x - 1, tile_y, 0xEA00)};
                        // // Buffer locations for kernel
                        // location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                        //     address(tile_x, tile_y, 0x0000),
                        //     address(tile_x, tile_y, 0x4000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                        //     address(tile_x, tile_y, 0x8000),
                        //     address(tile_x, tile_y, 0xC000)};
                    }
                    // All other AIEs in the group
                    else {

                        connect<>(A[plio_A].out[0], mat_mul_k[kernel_count].in[0]);
                        dimensions(mat_mul_k[kernel_count].in[0]) = {single_M * single_K *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: A[%d] to kernel_%d_in0 with dimension %d \n",
                            plio_A, kernel_count, single_M * single_K));

                        connect<>(B[plio_B].out[0], mat_mul_k[kernel_count].in[1]);
                        dimensions(mat_mul_k[kernel_count].in[1]) = {single_K * single_N *
                                                                     1};
                        DEBUG(printf(
                            "PL-Kernel: B[%d] to kernel_%d_in1 with dimension %d \n",
                            plio_B, kernel_count, single_K * single_N));

                        connect<cascade>(mat_mul_k[kernel_count].out[0],
                                         mat_mul_k[kernel_count + 1].in[2]);
                        DEBUG(printf("Cascade: kernel_%d_out0 to kernel_%d_in2 \n",
                                     kernel_count, kernel_count + 1));

                        source(mat_mul_k[kernel_count]) =
                            "kernels/kernels_bf16A_bf16B_i32PS_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        // location<stack>(mat_mul_k[kernel_count]) = {
                        //     address(tile_x - 1, tile_y, 0xEA00)};
                        // location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                        //     address(tile_x, tile_y, 0x0000),
                        //     address(tile_x, tile_y, 0x8000)};
                        // location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                        //     address(tile_x, tile_y, 0x4000),
                        //     address(tile_x, tile_y, 0xC000)};
                    }
                }
            }
        }
    }
};
