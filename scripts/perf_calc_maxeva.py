import argparse
import glob, os
from statistics import mean, variance, stdev


# Initialize the Parser
parser = argparse.ArgumentParser(description="Analyze AIE results.")

parser.add_argument(
    "--project_dir",
    type=str,
    help="Give the absolute project directory path. Emulation output data at: /<project_dir>/Emulation-AIE/aiesimulator_output/data",
)
parser.add_argument("--single_M", type=int, help="dim M of single MatMul kernel")
parser.add_argument("--single_K", type=int, help="dim K of single MatMul kernel")
parser.add_argument("--single_N", type=int, help="dim N of single MatMul kernel")
parser.add_argument("--mult_X", type=int, help="X MaxEVA parameter")
parser.add_argument("--mult_Y", type=int, help="Y MaxEVA parameter")
parser.add_argument("--mult_Z", type=int, help="Z MaxEVA parameter")
parser.add_argument("--precision", type=str, help='Give either "int8" or "fp32"')


args = parser.parse_args()

project_dir = args.project_dir

single_M = args.single_M
single_K = args.single_K
single_N = args.single_N

mult_X = args.mult_X
mult_Y = args.mult_Y
mult_Z = args.mult_Z

precision = args.precision


global_diff_list = []


for filename in glob.glob(project_dir + "/matC*.txt"):

    print("File:", filename)
    with open(filename) as file:

        # list that keeps the time of tlast on each file separetely
        file_tlast_list = []

        prevline = ""
        for line in file:
            if "TLAST" in line:
                time_list = prevline.split()

                if time_list[2] == "ns":
                    file_tlast_list.append(float(time_list[1]))
                elif time_list[2] == "ps":
                    file_tlast_list.append(float(time_list[1]) / 1000)
                elif time_list[2] == "us":
                    file_tlast_list.append(float(time_list[1]) * 1000)
                else:
                    print(time_list[2])
                    print("not ns or ps... exiting")
                    exit(1)

            prevline = line

        prev_time = file_tlast_list[0]
        file_diff_list = []
        iterations = len(file_tlast_list)
        for i in range(len(file_tlast_list)):

            if i > 0:
                file_diff_list.append(file_tlast_list[i] - prev_time)
                global_diff_list.append(file_diff_list[i - 1])

            prev_time = file_tlast_list[i]

# Calculate the mean of the group of 3 elements. Since we run for 4 iterations
op_avg = []
useful_iterations = iterations - 1
for i in range(0, len(global_diff_list), useful_iterations):
    group = global_diff_list[i : i + useful_iterations]
    op_avg.append(mean(group))


print("Average after taking mean of iterations: ", mean(op_avg))
print("Max after taking mean of iterations: ", max(op_avg))

print("File tlast len:", len(global_diff_list))
print("File tlast list:", global_diff_list)
print("Mean:", mean(global_diff_list))
Throughput = (
    2
    * (mult_X * single_M)
    * (mult_Y * single_K)
    * (mult_Z * single_N)
    / mean(global_diff_list)
)

Latency_avg = mean(global_diff_list) * 1e-9
Latency_max = max(global_diff_list) * 1e-9

Latency_avg_over_iter = mean(op_avg) * 1e-9
Latency_max_over_iter = max(op_avg) * 1e-9

print("")
print("Matrix size:", mult_X * single_M, "x", mult_Y * single_K, "x", mult_Z * single_N)

if precision == "int8":
    print("Precision: int8")
    print(f"Latency (avg over all iterations and outputs) AVG: {Latency_avg:.4e} s")
    print(f"Latency (max over all iterations and outputs) MAX: {Latency_max:.4e} s")

    print(
        f"Latency (avg over iter and avg over all outputs) AVG: {Latency_avg_over_iter:.4e} s"
    )
    print(
        f"Latency (max over iter and max over all outputs) MAX: {Latency_max_over_iter:.4e} s"
    )

    print("Throughput:", round(Throughput / 1000, 5), "TOPs")
elif precision == "fp32":
    print("Precision: fp32")
    print("Throughput:", round(Throughput, 5), "GFLOPs")
else:
    print("Give precision as either 'int8' or 'fp32'")
