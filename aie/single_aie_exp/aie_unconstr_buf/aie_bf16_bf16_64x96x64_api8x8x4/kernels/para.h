#ifndef PARA_H
#define PARA_H
#include <adf/stream/types.h>
#include <adf.h>
#include <aie_api/aie.hpp>
const int L0_h1 = 64;
const int L0_w1 = 96;
const int L0_w2 = 64;

using namespace adf;

void mm_kernel0(input_buffer<bfloat16> &__restrict matA,
                input_buffer<bfloat16> &__restrict matB,
                output_buffer<bfloat16> &__restrict matC);

#endif