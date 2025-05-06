#!/bin/bash

# Original folder
dir="aie/"
orig_folder=$dir"aie_bf16_bf16_64x64x64_api4x8x4"
echo $orig_folder
# Array of parameter values
workload_m=(32 32 32 64 64 64 64  64  128 128 128 32  32  128 128 )
workload_k=(32 64 64 32 64 64 128 128 128 64  128 128 128 128 32  )
workload_n=(32 32 64 64 32 64 64  128 64  128 128 32  128 32  128 )

# List of workload instructions in AIE-ML
mmul_m=(4 8 4)
mmul_k=(8 8 8)
mmul_n=(4 4 8)


# Loop to create copies
for j in ${!mmul_m[*]}
do
    for i in ${!workload_m[*]}
    do 
    # Get the new folder name from the parameter values
    new_folder=$dir"aie_bf16_bf16_${workload_m[i]}x${workload_k[i]}x${workload_n[i]}_api${mmul_m[j]}x${mmul_k[j]}x${mmul_n[j]}"
    echo $new_folder
    # Copy the original folder to the new folder
    mkdir -p "${new_folder}"
    cp -r "${orig_folder}"/* "${new_folder}"

    # Get the parameter values from the arrays
    h1=${workload_m[i]}
    w1=${workload_k[i]}
    w2=${workload_n[i]}

    mmul_h1=${mmul_m[j]}
    mmul_w1=${mmul_k[j]}
    mmul_w2=${mmul_n[j]}

    # Replace the values in the file
    sed -i "s/const int L0_h1 = 64;/const int L0_h1 = ${h1};/g" "${new_folder}/kernels/para.h"
    sed -i "s/const int L0_w1 = 64;/const int L0_w1 = ${w1};/g" "${new_folder}/kernels/para.h"
    sed -i "s/const int L0_w2 = 64;/const int L0_w2 = ${w2};/g" "${new_folder}/kernels/para.h"

    # Replace the values in the file
    sed -i "s/const int M = 4;/const int M = ${mmul_h1};/g" "${new_folder}/kernels/mm_kernel0.cc"
    sed -i "s/const int K = 8;/const int K = ${mmul_w1};/g" "${new_folder}/kernels/mm_kernel0.cc"
    sed -i "s/const int N = 4;/const int N = ${mmul_w2};/g" "${new_folder}/kernels/mm_kernel0.cc"

    /usr/bin/python3 scripts/sim_data_gen_bf16.py --dir $new_folder/data --M ${h1} --K ${w1} --N ${w2} --precision int8 --iter 4
    done
done