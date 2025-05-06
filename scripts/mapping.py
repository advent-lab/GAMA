import sys

# Map on a single row multiple column
X = 1
# Map the cascade connection
Y = 4
# Map on different rows
Z = 1

total_AIE = X * Y * Z


for i in range(X):
    for k in range(Z):
        # map on different rows
        for j in range(Y):
            # Select the AIE tile
            aie = (i * Y + j, k)
            print(f"AIE Tile {aie}")
            if j == 0:
                print(f"Kernel opt_blocked_matrix_mult_i8A_i8B_o32PS")
                print(f"Connect PLIO A[{j}] to Kernel_in0[{j}]")
                print(f"Connect PLIO B[{j}] to Kernel_in1[{j}]")
                print(f"Cascade Connection Kernel_in[{j}] to Kernel_out[{j+1}]")
            # The last AIE tile in the group
            elif j == Y - 1:
                print(f"Kernel opt_blocked_matrix_mult_i8A_i8B_i32PS_o16C")
                print(f"Connect PLIO A[{j}] to Kernel_in0[{j}]")
                print(f"Connect PLIO B[{j}] to Kernel_in1[{j}]")
                print(f"Connect Kernel_out[{j}] to PLIO C[{j%3}]")
            # AIE in middle
            else:
                print(f"Kernel opt_blocked_matrix_mult_i8A_i8B_i32PS_o32PS")
                print(f"Connect PLIO A[{j}] to Kernel_in0[{j}]")
                print(f"Connect PLIO B[{j}] to Kernel_in1[{j}]")
                print(f"Cascade Connection Kernel_in[{j}] to Kernel_out[{j+1}]")
