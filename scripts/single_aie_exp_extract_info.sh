
# Unconstrained single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_unconstr_buf/aie_i8_i32_48x240x48_api4x8x8_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_unconstr_buf/aie_i8_i16_64x184x64_api4x8x8_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_unconstr_buf/aie_i8_i8_64x224x64_api4x8x8_no_pl_no_host_BufOpt8_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_unconstr_buf/aie_bf16_bf16_64x96x64_api8x8x4_no_pl_no_host_BufOpt8_hw/ | grep Average  


# Confined single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_conf_buf/aie_i8_i32_48x240x48_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_conf_buf/aie_i8_i16_64x184x64_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_conf_buf/aie_i8_i8_64x224x64_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_conf_buf/aie_bf16_bf16_64x96x64_api8x8x4_no_pl_no_host_BufOpt0_hw/ | grep Average  


# Custom Buffer Constrained single AIE experiments
python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_constr_buf/aie_i8_i32_48x240x48_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_constr_buf/aie_i8_i16_64x184x64_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_constr_buf/aie_i8_i8_64x224x64_api4x8x8_no_pl_no_host_BufOpt0_hw/ | grep Average  

python ./scripts/extract_avg_from_profile.py --kernel mm_kernel0 --dir sim/aie/single_aie_exp/aie_constr_buf/aie_bf16_bf16_64x96x64_api8x8x4_no_pl_no_host_BufOpt0_hw/ | grep Average  
