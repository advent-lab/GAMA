
# Unconstrained single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ | grep Average  


# Confined single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  


# Custom Buffer Constrained single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel opt_blocked_matrix_mult --dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ | grep Average  


# Extract latency from output.txt 
# Unconstrained single AIE experiments
python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ --single_M 48 --single_K 240 --single_N 48 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ --single_M 64 --single_K 184 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ --single_M 64 --single_K 224 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_unconstr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt8_hw/ --single_M 64 --single_K 96 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  


# Confined single AIE experiments
python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 48 --single_K 240 --single_N 48 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 184 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 224 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_conf_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 96 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  


# Custom Buffer Constrained single AIE experiments
python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_48x240x48_MMUL_4x8x8_i8_i32_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 48 --single_K 240 --single_N 48 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x184x64_MMUL_4x8x8_i8_i16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 184 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x224x64_MMUL_4x8x8_i8_i8_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 224 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:'  

python3 ./scripts/perf_calc_maxeva.py --project_dir sim/aie/pack_aie_exp/aie_constr_buf/aie_MKN_64x96x64_MMUL_8x8x4_bf16_bf16_X1_Y4_Z1_no_pl_no_host_BufOpt0_hw/ --single_M 64 --single_K 96 --single_N 64 --mult_X 1 --mult_Y 4 --mult_Z 1 --precision int8 | grep 'Latency (avg over all iterations and outputs) AVG:' 
