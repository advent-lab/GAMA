#!/bin/bash

# Original folder
dir="aie/"
orig_folder=$dir"aie_i8_i8_128x128x128_api4x8x8"
echo $orig_folder
# Array of parameter values
# mmul_m=(32 64 8 8 8 8 8 8 16 16 16 16 16 16 16 16 16 16 32 32 32 32 32 32 32 64 64 64 64 64 64 64 128 128 128 128 160)
# mmul_k=(32 64 32 32 64 64 128 128 64 128 32 16 16 32 16 64 16 128 8 16 32 32 32 64 64 8 16 32 64 64 64 64 8 16 128 128 128)
# mmul_n=(32 64 8 32 8 64 8 128 16 16 16 16 16 32 16 64 16 128 32 32 8 16 32 32 64 64 64 64 8 16 32 64 128 128 8 16 128)

# List of MMUL instructions in AIE-ML
mmul_m=(4 4 8 2 4 8 1 2 4)
mmul_k=(8 16 8 8 8 8 16 16 16)
mmul_n=(4 4 4 8 8 8 8 8 8)


# Loop to create copies
for i in ${!mmul_m[*]}
do
    # Get the new folder name from the parameter values
    new_folder=$dir"aie_i8_i8_128x128x128_api${mmul_m[i]}x${mmul_k[i]}x${mmul_n[i]}"
    echo $new_folder
    # Copy the original folder to the new folder
    mkdir -p "${new_folder}"
    cp -r "${orig_folder}"/* "${new_folder}"

    # Get the parameter values from the arrays
    h1=${mmul_m[i]}
    w1=${mmul_k[i]}
    w2=${mmul_n[i]}

    # Replace the values in the file
    sed -i "s/const int M = 4;/const int M = ${h1};/g" "${new_folder}/kernels/mm_kernel0.cc"
    sed -i "s/const int K = 8;/const int K = ${w1};/g" "${new_folder}/kernels/mm_kernel0.cc"
    sed -i "s/const int N = 8;/const int N = ${w2};/g" "${new_folder}/kernels/mm_kernel0.cc"
done