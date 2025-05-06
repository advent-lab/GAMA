
# Unconstrained single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_constr_buf_array_scaling/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X8_Y4_Z9  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_X8_Y4_Z9.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_constr_buf_array_scaling/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X8_Y4_Z9  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_X8_Y4_Z9.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_constr_buf_array_scaling/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X8_Y4_Z9  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_X8_Y4_Z9.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_constr_buf_array_scaling/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X8_Y4_Z9  TARGET=hw BUFF_OPT=0 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_X8_Y4_Z9.log



