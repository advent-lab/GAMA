import os  
import sys
import numpy as np
import argparse

parser = argparse.ArgumentParser(description ='Generate Synthetic data.')

parser.add_argument('--dir', type=str, help='The directory to write the data to')
parser.add_argument('--M', type=int, help='dim M of single MatMul kernel')
parser.add_argument('--K', type=int, help='dim K of single MatMul kernel')
parser.add_argument('--N', type=int, help='dim N of single MatMul kernel')
parser.add_argument('--precision', type=str, help='Give either "int8" or "fp32"')
parser.add_argument('--iter', type=str, help='Iteration count')


args = parser.parse_args()
project_dir = args.dir
iter = args.iter

M = args.M
K = args.K
N = args.N

precision = args.precision
ELEMENT_PER_ROW = 16

# a = np.random.randint(0,2,size=(M,K),dtype=np.uint8)

# b = np.random.randint(0,2,size=(K,N),dtype=np.uint8)

a = np.ones(M*K,dtype=np.uint8)

b = np.ones(K*N,dtype=np.uint8)
# print(a)
# print(b)
a_flat = a.flatten()
b_flat = a.flatten()
res = np.dot(a,a)
res_flat = res.flatten()
# print(res)

# Write input0.txt 
f_dir = project_dir + "/input0.txt"
print(f_dir)
f = open(f_dir, "w")
for q in range(0,int(iter)):
    for i in range(0,int(M*K/ELEMENT_PER_ROW)-1):
        # if i == 0:
        #     f.write("2415853568 0 0 0 ")
        #     for j in range(0,12):
        #         f.write(str(a_flat[j]) + ' ')
        #     f.write('\n')
        # else:
        for j in range(0,16):
            f.write("1" + ' ')
            # f.write(str(a_flat[((i-1)*ELEMENT_PER_ROW + j) + 12]) + ' ')
        f.write('\n')
            
    f.write("TLAST\n")
    # f.write(str(a_flat[-4]) + ' ' + str(a_flat[-3]) + ' ' + str(a_flat[-2]) + ' ' + str(a_flat[-1])+' 0 0 0 0 0 0 0 0 0 0 0 0 \n')
    for j in range(0,16):
        f.write("1" + ' ')    
        # f.write(str(a_flat[((i-1)*ELEMENT_PER_ROW + j) + 12]) + ' ') 
    f.write('\n')  
f.close()

# Write input1.txt 
f_dir = project_dir + "/input1.txt"
print(f_dir)
f = open(f_dir, "w")
for q in range(0,int(iter)):
    for i in range(0,int(K*N/ELEMENT_PER_ROW)-1):
        # if i == 0:
        #     f.write("2415853568 0 0 0 ")
        #     for j in range(0,12):
        #         f.write(str(b_flat[j]) + ' ')
        #     f.write('\n')
        #     # + str(1) + ' ' + str(1) + ' ' + str(1)+' '+ str(1)+' '+ str(1) + ' ' + str(1) + ' ' + str(1)+' '+ str(1)+ ' ' + str(1) + ' ' + str(1)+' '+ str(1)+' '+ str(1) + ' ' + str(1) + ' ' + str(1)+' '+ str(1) +'\n')
        # else:
        for j in range(0,16):
            f.write("1" + ' ')  
            # f.write(str(b_flat[((i-1)*ELEMENT_PER_ROW + j) + 12]) + ' ')
        f.write('\n')
            
    f.write("TLAST\n")
    # f.write(str(b_flat[-4]) + ' ' + str(b_flat[-3]) + ' ' + str(b_flat[-2]) + ' ' + str(b_flat[-1])+' 0 0 0 0 0 0 0 0 0 0 0 0 \n')
    for j in range(0,16):
        f.write("1" + ' ')  
        # f.write(str(b_flat[((i-1)*ELEMENT_PER_ROW + j) + 12]) + ' ')
    f.write('\n')  
f.close()


# Write golden_output.txt 
# f = open("data/golden_output.txt", "w")
# for i in range(0,int(M*N/ELEMENT_PER_ROW)):
#     f.write("T 11504808 ps\n")
#     if ( i == (M*N/ELEMENT_PER_ROW-1)):
#         f.write("TLAST\n")
#     for j in range(0,16):
#         f.write(str(res_flat[i*ELEMENT_PER_ROW + j]) + ' ')
#     f.write('\n')
# f.close()