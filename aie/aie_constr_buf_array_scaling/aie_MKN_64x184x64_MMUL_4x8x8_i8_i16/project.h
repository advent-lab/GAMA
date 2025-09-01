
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
    kernel mat_mul_k[mult_Y * mult_G * mult_X];

public:
    input_plio A[mult_Y * mult_G];
    input_plio B[mult_G * mult_X];
    output_plio C[mult_Y * mult_X];

    simpleGraph() {

        DEBUG(printf("Debug simpleGraph AIE placement and buffer allocation\n"));
        DEBUG(printf("Scaling: Y%d x G%d x X%d\n", mult_Y, mult_G, mult_X));	
 	DEBUG(printf("Total AIEs: %d\n", mult_Y * mult_G * mult_X));
        DEBUG(printf("Total PLIOs: %d (IN %d OUT %d)\n",
                     (mult_Y * mult_G + mult_G * mult_X + mult_Y * mult_X),
                     (mult_Y * mult_G + mult_G * mult_X), mult_Y * mult_X));
        // input and output plio creation below
        // A: 0   1    2  ...
        //    x  x+1  x+2 ...
        for (int i = 0; i < mult_Y * mult_G; i++) {
            A[i] = input_plio::create("A" + std::to_string(i), plio_128_bits,
                                      "./matA" + std::to_string(i) + ".txt");
            DEBUG(printf("PLIO A[%d] file: ./matA%d.txt\n", i, i));
        }

        // B: 0   y
        //    1  y+1
        //    2  y+2
        //   ... ...
        for (int i = 0; i < mult_G * mult_X; i++) {
            B[i] = input_plio::create("B" + std::to_string(i), plio_128_bits,
                                      "./matB" + std::to_string(i) + ".txt");
            DEBUG(printf("PLIO B[%d] file: ./matB%d.txt\n", i, i));
        }

        // C: 0   1    2  ...
        //    x  x+1  x+2 ...
        for (int i = 0; i < mult_Y * mult_X; i++) {
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

        // mult_X = 4
        // mult_Y = 1

        // X dimension scales the AIEs over the rows
        for (int j = 0; j < mult_Y; j++) {
            // Z dimension scales the AIEs over the column
            for (int k = 0; k < mult_X; k++) {
                // Use Y axis for grouping the AIE's for reduction
                for (int i = 0; i < mult_G; i++) {
                    k_count_ka = (j * mult_X * mult_G) + (k * mult_G) + i;
                    DEBUG(printf("Kernel count for kernel assignment: %d\n", k_count_ka));
                    if (i == 0) {
                        mat_mul_k[k_count_ka] =
                            kernel::create(opt_blocked_matrix_mult_i8A_i8B_o32PS);
                    } else if (i == mult_G - 1) {
                        mat_mul_k[k_count_ka] =
                            kernel::create(opt_blocked_matrix_mult_i8A_i8B_i32PS_o16C);
                    } else {
                        mat_mul_k[k_count_ka] =
                            kernel::create(opt_blocked_matrix_mult_i8A_i8B_i32PS_o32PS);
                    }
                }

                // Kernel creation below
                for (int i = 0; i < mult_G; i++) {
                    // tile_x = (k * mult_G) + i;
                    if (j % 2) {
                        tile_x = (k * mult_G) + i + 2;
                    } else {
                        tile_x = (k * mult_G) + i;
                    }
                    tile_y = j;
                    // kernel_count = i;
                    kernel_count = (j * mult_X * mult_G) + (k * mult_G) + i;
                    plio_A = (j * mult_G) + i;
                    plio_B = (k * mult_G) + i;
                    // plio_C = i % (mult_G - 1); // One PLIO out for each group
                    plio_C = j * mult_X + k; // One PLIO out for each group
                    DEBUG(printf("AIE_Tile(%d,%d)\n", tile_x, tile_y));
                    DEBUG(printf("Kernel %d\n", kernel_count));
                    DEBUG(printf("PLIO(A[%d],B[%d])\n", plio_A, plio_B));

                    // Kernel 0 should be opt_blocked_matrix_mult_i8A_i8B_o32PS
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
                            "kernels/kernels_i8A_i8B_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        location<stack>(mat_mul_k[kernel_count]) = {
                            address(tile_x, tile_y, 0x2E00)};
                        location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                            address(tile_x, tile_y, 0x0000),
                            address(tile_x, tile_y, 0x4000)};
                        location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                            address(tile_x, tile_y, 0x8000),
                            address(tile_x, tile_y, 0xC000)};
                    }
                    // Last kernel in the group should have outputs to PLIO
                    else if (i == mult_G - 1) {

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
                            "kernels/kernels_i8A_i8B_i32PS_o16C.cc";

                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 1 buffer locations
                        location<stack>(mat_mul_k[kernel_count]) = {
                            address(tile_x, tile_y, 0x2E00)};
                        location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                            address(tile_x, tile_y, 0x0),
                            address(tile_x, tile_y, 0x8000)};
                        location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                            address(tile_x, tile_y, 0x4000),
                            address(tile_x, tile_y, 0xC000)};
                        location<buffer>(mat_mul_k[kernel_count].out[0]) = {
                            address(tile_x - 1, tile_y, 0x2E00),
                            address(tile_x - 1, tile_y, 0xAE00)};
                    }
                    // Kernel in between should have cascade input and output for partial
                    // sum
                    else if (i == mult_G - 2) {

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
                            "kernels/kernels_i8A_i8B_i32PS_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        location<stack>(mat_mul_k[kernel_count]) = {
                            address(tile_x - 1, tile_y, 0xEF00)};
                        // Buffer locations for kernel
                        location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                            address(tile_x, tile_y, 0x0000),
                            address(tile_x, tile_y, 0x8000)};
                        location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                            address(tile_x, tile_y, 0x4E00),
                            address(tile_x, tile_y, 0xCE00)};
                    } else {

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
                            "kernels/kernels_i8A_i8B_i32PS_o32PS.cc";
                        location<kernel>(mat_mul_k[kernel_count]) = tile(tile_x, tile_y);
                        runtime<ratio>(mat_mul_k[kernel_count]) = 1.0;

                        // Kernel 0 buffer locations
                        location<stack>(mat_mul_k[kernel_count]) = {
                            address(tile_x - 1, tile_y, 0xEE00)};
                        location<buffer>(mat_mul_k[kernel_count].in[0]) = {
                            address(tile_x, tile_y, 0x0000),
                            address(tile_x, tile_y, 0x8000)};
                        location<buffer>(mat_mul_k[kernel_count].in[1]) = {
                            address(tile_x, tile_y, 0x4000),
                            address(tile_x, tile_y, 0xC000)};
                    }
                }
            }
        }
    }
};
