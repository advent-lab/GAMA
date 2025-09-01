#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "include.h"
#include <adf.h>
#include <aie_api/utils.hpp>
using namespace adf;

/*
 *  Matrices should be in blocked format in memory, in the following order:
 * 	 ____________________________
 * 	|  1 |  2 |  3 | ...
 * 	|____|____|____|
 * 	|  x | x+1| x+2| ...
 * 	|____|____|____|
 * 	|.
 * 	|.
 * 	|.
 *
 * 	Tile size is defined from the AI Engine APIs
 *  check include.h file, dimensions named M_API, K_API, N_API
 *
 */
const int M = 8;
const int K = 8;
const int N = 4;

const unsigned int num_rowA = L0_h1 / M;
const unsigned int num_colA = L0_w1 / K;
const unsigned int num_colB = L0_w2 / N;

// optimized matrix multiplication kernel
void opt_blocked_matrix_mult_bf16A_bf16B_i32PS_o32C(
    input_circular_buffer<bfloat16> &__restrict matA,
    input_circular_buffer<bfloat16> &__restrict matB, input_cascade<accfloat> *ps_in,
    output_buffer<bfloat16> &__restrict matC)
{

    using MMUL = aie::mmul<M, K, N, bfloat16, bfloat16>;

    // printf("In Kernel Aie \n");
    const bfloat16 *__restrict pA = matA.data();
    const bfloat16 *__restrict pB = matB.data();
    bfloat16 *__restrict pC = matC.data();
    aie::accum<accfloat, MMUL::size_C> acc1;

    for (unsigned int i = 0; i < num_rowA; i += 2)
        chess_prepare_for_pipelining
        {
            bfloat16 *__restrict pC1 = pC + (i * num_colB) * MMUL::size_C;
            bfloat16 *__restrict pC2 = pC + ((i + 1) * num_colB) * MMUL::size_C;
            // aie::print(pC1, true, "pC1=");
            // aie::print(pC2, true, "pC2=");

            // printf("pC1= %d\n", (i * num_colB) * MMUL::size_C);
            // printf("pC2= %d\n", ((i + 1) * num_colB) * MMUL::size_C);

            for (unsigned int j = 0; j < num_colB; j += 2)
                chess_prepare_for_pipelining
                {
                    const bfloat16 *__restrict pA1 = pA + (i * num_colA) * MMUL::size_A;
                    const bfloat16 *__restrict pA2 =
                        pA + ((i + 1) * num_colA) * MMUL::size_A;

                    // aie::print(pA1, true, "pA1=");
                    // aie::print(pA2, true, "pA2=");
                    // printf("pA1= %d\n", (i * num_colA + 0) * MMUL::size_A);
                    // printf("pA2= %d\n", ((i + 1) * num_colA + 0) * MMUL::size_A);

                    const bfloat16 *__restrict pB1 = pB + j * MMUL::size_B;
                    const bfloat16 *__restrict pB2 = pB + (j + 1) * MMUL::size_B;

                    // printf("pB1= %d\n", (j)*MMUL::size_B);
                    // printf("pB2= %d\n", (j + 1) * MMUL::size_B);

                    aie::vector<bfloat16, MMUL::size_A> A0 =
                        aie::load_v<MMUL::size_A>(pA1);
                    pA1 += MMUL::size_A;
                    // aie::print(A0,true,"A0=");

                    aie::vector<bfloat16, MMUL::size_A> A1 =
                        aie::load_v<MMUL::size_A>(pA2);
                    pA2 += MMUL::size_A;
                    // aie::print(A1,true,"A1=");

                    aie::vector<bfloat16, MMUL::size_B> B0 =
                        aie::load_v<MMUL::size_B>(pB1);
                    pB1 += MMUL::size_B * num_colB;
                    // aie::print(B0,true,"B0=");

                    aie::vector<bfloat16, MMUL::size_B> B1 =
                        aie::load_v<MMUL::size_B>(pB2);
                    pB2 += MMUL::size_B * num_colB;
                    // aie::print(B1,true,"B1=");
                    // printf("Last kernel\n");
                    ps_in >> acc1;
                    // aie::print(acc1, true, "C00_acc1=");
                    MMUL C00(acc1);
                    ps_in >> acc1;
                    // aie::print(acc1, true, "C01_acc1=");
                    MMUL C01(acc1);
                    ps_in >> acc1;
                    // aie::print(acc1, true, "C10_acc1=");
                    MMUL C10(acc1);
                    ps_in >> acc1;
                    // aie::print(acc1, true, "C11_acc1=");
                    MMUL C11(acc1);

                    // matrix multiply by initializing to 0
                    C00.mac(A0, B0);
                    C01.mac(A0, B1);
                    C10.mac(A1, B0);
                    C11.mac(A1, B1);

                    for (unsigned int k = 0; k < num_colA - 1; k++)
                        chess_prepare_for_pipelining
                        {
                            A0 = aie::load_v<MMUL::size_A>(pA1);
                            pA1 += MMUL::size_A;
                            // aie::print(A0,true,"A0=");
                            A1 = aie::load_v<MMUL::size_A>(pA2);
                            pA2 += MMUL::size_A;
                            // aie::print(A1,true,"A1=");

                            B0 = aie::load_v<MMUL::size_B>(pB1);
                            pB1 += MMUL::size_B * num_colB;
                            // aie::print(B0,true,"B0=");
                            B1 = aie::load_v<MMUL::size_B>(pB2);
                            pB2 += MMUL::size_B * num_colB;
                            // aie::print(B1,true,"B1=");

                            // matrix multiply and adding partial blocks
                            C00.mac(A0, B0);
                            C01.mac(A0, B1);
                            C10.mac(A1, B0);
                            C11.mac(A1, B1);
                        }

                    aie::store_v(pC1, C00.template to_vector<bfloat16>());
                    pC1 += MMUL::size_C;
                    aie::store_v(pC1, C01.template to_vector<bfloat16>());
                    pC1 += MMUL::size_C;
                    aie::store_v(pC2, C10.template to_vector<bfloat16>());
                    pC2 += MMUL::size_C;
                    aie::store_v(pC2, C11.template to_vector<bfloat16>());
                    pC2 += MMUL::size_C;
                }
        }
}
