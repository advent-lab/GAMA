
#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

using namespace adf;

// void opt_blocked_matrix_mult(input_circular_buffer<int8> &__restrict matA,
// input_circular_buffer<int8> &__restrict matB,
// output_buffer<int16> &__restrict matC);

// void vectorized_add(input_buffer<int16> &__restrict in_1, input_buffer<int16>
// &__restrict in_2, 					output_buffer<int16> &__restrict out); void
// opt_blocked_matrix_mult(input_circular_buffer<int8> &__restrict matA,
// 							 input_circular_buffer<int8> &__restrict matB,
// 							 output_cascade<int32> &__restrict matC,
// 							 //  output_buffer<int16> &__restrict matC
// );
void opt_blocked_matrix_mult_i8A_i8B_o32PS(input_circular_buffer<int8> &__restrict matA,
                                           input_circular_buffer<int8> &__restrict matB,
                                           output_cascade<acc32> *matC);
void opt_blocked_matrix_mult_i8A_i8B_i32PS_o16C(
    input_circular_buffer<int8> &__restrict matA,
    input_circular_buffer<int8> &__restrict matB, input_cascade<acc32> *ps_in,
    output_buffer<int16> &__restrict matC);

void opt_blocked_matrix_mult_i8A_i8B_i32PS_o32PS(
    input_circular_buffer<int8> &__restrict matA,
    input_circular_buffer<int8> &__restrict matB, input_cascade<acc32> *ps_in,
    output_cascade<acc32> *ps_out);

#endif
