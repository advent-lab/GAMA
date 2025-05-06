#include "kernels/aie_top.h"
using namespace adf;

mm_graph mm_graph0;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv)
{
    std::cout << "Simulation started!!";
    mm_graph0.init();
    mm_graph0.run(4);
    mm_graph0.end();
    return 0;
}
#endif