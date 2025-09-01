#ifndef FUNCTION_INCLUDES_H
#define FUNCTION_INCLUDES_H

// define shift right for output values after matrix mult
#define SHIFT 0

// X dimesion scales the AIEs over the rows
#define mult_Y 1
// Y dimension is used to group AIEs for reduction
#define mult_G 4
// Z dimension scales the AIEs over the columns
#define mult_X 1

// single kernel dimensions (MxKxN on manuscript)
#define single_M 64
#define single_K 96
#define single_N 64

// AI Engine API dimensions
#define M_API 8
#define K_API 8
#define N_API 4

const int L0_h1 = 64;
const int L0_w1 = 96;
const int L0_w2 = 64;

#define SW_SIM_ITER 4
#endif
