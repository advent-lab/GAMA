# GAMA: High-Performance GEMM Acceleration on AMD Versal ML-Optimized AI Engines

arxiv : https://arxiv.org/abs/2504.09688

## Overview
GAMA is an open-source framework delivering state-of-the-art acceleration for General Matrix-Matrix Multiplication (GEMM) workloads on AMD’s AIE-ML architecture (second generation), specifically optimized for machine learning applications. GAMA achieves up to 165 TOPS (85%) for int8 and 83 TBFLOPS (86% peak) for bfloat16 precisions, outperforming existing AIE1 (first generation) frameworks.

| Precision(ip-op) | MxKxN | Throughput(TOPs/TBFLOPs) | Throughput Efficiency |
|:----:|:---------------:|:------------------:|:------------------:|
|int8-int32 | 384x960x432 | 133 | 69% |
|int8-int16 | 512x736x576 | 159 | 82% |
|int8-int8 | 512x896x576 | 165 | 85% |
|bf16-bf16 | 512x384x576 | 83 | 86% |

### Features
* Comprehensive GEMM acceleration for AMD Versal AIE-ML (VEK280)
* Supports int8 and bfloat16 precisions
* Maximizes the memory utilization upto 100%  
* Custom buffer placement algorithm that minimizes bank conflicts reducing memory stalls
* Staggered kernel placement pattern across AIE arrays to maximize AIE array utilization upto 94%

### Architecture Overview
GAMA operates on the AMD Versal VE2802 device, leveraging 304 AIE-ML cores (8×38 grid) and supporting PLIO interfaces for communication with Programmable Logic (PL). The framework implements designs at three levels:

* Single AIE: Maximizes memory utilization by selecting optimal kernel sizes for different precisions
* Pack of AIEs: Chains AIEs leveraging the cascade interface to solve a bigger problem size
* Complete Array: Replicates packs with staggered placement to avoid routing congestion and ensure scalability

### Scaling hyperparameters: X, G and Y 

GAMA's design can be scaled using the hyperparameters listed below
Eg: X=2, G=4, Y=2

Here is how the workload is mapped to the AIEs using multiple packs
<img width="1704" height="347" alt="AIE_scaling_2x4x2_v3" src="https://github.com/user-attachments/assets/8df8298f-3d3a-4c1c-942d-fd2e4827e9f9" />

This design gets mapped to the grid as shown below
<img width="1398" height="296" alt="mapping_to_array" src="https://github.com/user-attachments/assets/058cf37f-b1ee-48a1-89c1-f5f673099963" />


### Dependencies
* AMD/Xilinx Vitis 2024.1
* AMD/Xilinx Versal VEK280 Platfrom (xilinx_vek280_base_202410_1.xpfm)



### Environment Setup
```bash
source /mnt/vault1/kmhatre/Software/AMD/Vitis/2024.1/settings64.sh
export XILINX_VERSAL=/opt/AMD/common_files/xilinx-versal-common-v2024.1/
source $XILINX_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux

export ROOTFS=/opt/AMD/common_files/xilinx-versal-common-v2024.1/rootfs.ext4  
export IMAGE=/opt/AMD/common_files/xilinx-versal-common-v2024.1/Image
export PLATFORM_REPO_PATHS=/mnt/vault1/kmhatre/Software/AMD/Vitis/2024.1/base_platforms/

source /opt/xilinx/xrt/setup.sh
```

### Compilation command 
``` bash
make -f Makefile_VEK280 aiesim AIE_SRC=<path to the aie directory> TARGET=hw Y=8 G=4 X=9 BUFF_OPT=0
eg: make -f Makefile_VEK280 AIE_SRC=aie/aie_constr_buf_array_scaling/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32  TARGET=hw Y=8 G=4 X=9 BUFF_OPT=0 aiesim
```



## Cite 
 ```bash
@inproceedings{Mhatre2025GAMA,
  title={GAMA: High-Performance GEMM Acceleration on AMD Versal ML-Optimized AI Engines},
  author={Mhatre, Kaustubh and Taka, Endri and Arora, Aman},
  booktitle={2025 International Conference on Field-Programmable Logic and Applications (FPL)},
  year={2025},
  organization={IEEE},
}
```
