
# Unconstrained single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_unconstr_buf/aie_i8_i32_48x240x48_api4x8x8  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_ucb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_unconstr_buf/aie_i8_i16_64x184x64_api4x8x8  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_ucb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_unconstr_buf/aie_i8_i8_64x224x64_api4x8x8  TARGET=hw BUFF_OPT=8 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_ucb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_unconstr_buf/aie_bf16_bf16_64x96x64_api8x8x4  TARGET=hw BUFF_OPT=8 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_ucb.log


# Confined single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_conf_buf/aie_i8_i32_48x240x48_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_confb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_conf_buf/aie_i8_i16_64x184x64_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_confb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_conf_buf/aie_i8_i8_64x224x64_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_confb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_conf_buf/aie_bf16_bf16_64x96x64_api8x8x4  TARGET=hw BUFF_OPT=0 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_confb.log


# Custom Buffer Constrained single AIE experiments
make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_constr_buf/aie_i8_i32_48x240x48_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i32_48x240x48_api4x8x8_constrb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_constr_buf/aie_i8_i16_64x184x64_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i16_64x184x64_api4x8x8_constrb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_constr_buf/aie_i8_i8_64x224x64_api4x8x8  TARGET=hw BUFF_OPT=0 &> compile_log/aie_i8_i8_64x224x64_api4x8x8_constrb.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/single_aie_exp/aie_constr_buf/aie_bf16_bf16_64x96x64_api8x8x4  TARGET=hw BUFF_OPT=0 &> compile_log/aie_bf16_bf16_64x96x64_api8x8x4_constrb.log
