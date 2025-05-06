#ifndef PARA_H
#define PARA_H
#include <adf.h>
#include <adf/stream/types.h>
#include <aie_api/aie.hpp>
const int L0_h1 = 48;
const int L0_w1 = 240;
const int L0_w2 = 48;
const int DATA_INT16 = 2;

using namespace adf;

void mm_kernel0(input_buffer<int8> &__restrict matA, input_buffer<int8> &__restrict matB,
                output_buffer<int32> &__restrict matC);

#endif