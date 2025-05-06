
# Unconstrained single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_ucb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_ucb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_ucb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1  TARGET=hw BUFF_OPT=8 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_ucb_pack.log


# Confined single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_conf_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_confb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_confb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_confb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_confb_pack.log


# Custom Buffer Constrained single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_constr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_constrb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_constrb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_constrb_pack.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_constrb_pack.log
