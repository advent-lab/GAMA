for i in {3..38} 
    do 
        # echo "Running AIE with Y = $i"
        python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir ./sim/aie/aie_g_var_sweep/aie_MKN_48x224x48_MMUL_4x8x8_i8_i32_X1_Y${i}_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average; 
done


#  make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_g_var_sweep/aie_MKN_48x224x48_MMUL_4x8x8_i8_i32_X1_Y16_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_MKN_48x224x48_MMUL_4x8x8_i8_i32_X1_Y16_Z1.log

make -f Makefile_VEK280 aiesim AIE_SRC=aie/aie_g_var_sweep/aie_MKN_48x224x48_MMUL_4x8x8_i8_i32_X1_Y2_Z1  TARGET=hw BUFF_OPT=0 &> compile_log/aie_MKN_48x224x48_MMUL_4x8x8_i8_i32_X1_Y2_Z1.log