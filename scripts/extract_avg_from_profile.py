import os
import re
import argparse


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Extract opt_blocked_matrix_mult values from log files."
    )
    parser.add_argument(
        "--dir", type=str, help="Path to the directory containing log files"
    )
    parser.add_argument("--kernel", type=str, help="kernel name")
    parser.add_argument(
        "--kernel_iter", type=int, help="Iterations of kernel default 4", default=4
    )
    return parser.parse_args()


def extract_opt_blocked_matrix_mult(file_path, kernel_name):
    """
    Extracts the opt_blocked_matrix_mult value from the given log file.
    """
    opt_value = None
    pattern = r"opt_blocked_matrix_mult\s.*?\s(\d+)"

    # opt_blocked_matrix_mult
    # mm_kernel0

    with open(file_path, "r") as file:
        for line in file:
            if kernel_name in line:
                opt_value = int(line.split()[1])
                if opt_value == None:
                    pass
                # match = re.search(pattern, line)
                # if match:
                #     opt_value = int(match.group(1))
                break  # Exit loop once the value is found
    return opt_value


def process_files_in_directory(directory_path, kernel_name):
    """
    Processes all log files in the given directory and extracts the opt_blocked_matrix_mult values.
    """
    results = {}
    for root, _, files in os.walk(directory_path):
        for file_name in files:
            # if file_name.endswith(".txt"):  # Assuming log files have '.log' extension
            if "funct" in file_name and ".txt" in file_name:
                # print(file_name)
                file_path = os.path.join(root, file_name)
                opt_value = extract_opt_blocked_matrix_mult(file_path, kernel_name)
                results[file_path] = opt_value
    return results


def main():
    args = parse_arguments()

    # Path to the directory containing log files
    # directory_path = (
    #     "sim/aie_maxeva_XYZ_4x2x4_MKN_48x192x48_MMUL_4x8x8_i8_i32_my_kernel_hw"
    # )
    directory_path = args.dir
    kernel_name = args.kernel
    kernel_iter = args.kernel_iter
    # Process the files and get the results
    results = process_files_in_directory(directory_path, kernel_name)
    value_list = []
    for file_path, value in results.items():
        if value is not None:
            print(f"File: {file_path}, opt_blocked_matrix_mult: {value}")
            value_list.append(value)

    print(
        f"Average {kernel_name}: {sum(value_list)/len(value_list)} divide by iter {kernel_iter} = {sum(value_list)/len(value_list)/kernel_iter}"
    )

    # Print or save the results
    # for file_path, value in results.items():
    #     if value is not None:
    #         print(f"File: {file_path}, opt_blocked_matrix_mult: {value}")
    #     else:
    #         print(f"File: {file_path}, opt_blocked_matrix_mult: Not Found")


if __name__ == "__main__":
    main()
