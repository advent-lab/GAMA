// #include "aie_graph_L0.h"
#include <adf.h>
#include "para.h"
#include <stdio.h>
using namespace adf;

class mm_graph : public adf::graph {
public:
    input_plio in_lhs[1];
    input_plio in_rhs[1];
    output_plio out[1];

    kernel mm[1];

    mm_graph() {

        in_lhs[0] = input_plio::create("LHS_in0_L0", adf::plio_128_bits, "./input0.txt");
        in_rhs[0] = input_plio::create("RHS_in0_L0", adf::plio_128_bits, "./input1.txt");
        out[0] = output_plio::create("out0_L0", adf::plio_128_bits, "./output0.txt");

        mm[0] = kernel::create(mm_kernel0);
        source(mm[0]) = "mm_kernel0.cc";
        runtime<ratio>(mm[0]) = 1;

        connect<>(in_lhs[0].out[0], mm[0].in[0]);
        adf::dimensions(mm[0].in[0]) = {L0_h1 * L0_w1};
        connect<>(in_rhs[0].out[0], mm[0].in[1]);
        adf::dimensions(mm[0].in[1]) = {L0_w1 * L0_w2};

        connect<>(mm[0].out[0], out[0].in[0]);
        adf::dimensions(mm[0].out[0]) = {L0_h1 * L0_w2};

        location<buffer>(mm[0].in[0]) = location<kernel>(mm[0]);

        location<buffer>(mm[0].in[1]) = location<kernel>(mm[0]);
        location<buffer>(mm[0].out[0]) = location<kernel>(mm[0]);
    }
};