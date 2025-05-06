# AIE engine events parsing scripts
import os
import sys
import argparse
import re
import csv


class Tile:
    def __init__(self, col, row):
        self.col = col
        self.row = row

    def __repr__(self):
        return f"Tile({self.col},{self.row})"


class log_entry:
    def __init__(self, log_line, time, event, tile, lockid, lockValue):
        self.log_txt = log_line
        self.time = time
        self.event = event
        self.tile = tile
        self.lockid = lockid
        self.lockValue = lockValue

    def __str__(self):
        return self.log_txt

    def __repr__(self):
        return self.log_txt


ptrn_lockid_alloc = r"lockid=(\d+)"
ptrn_lockid_mm2s = r"lock_id=(\d+)"
ptrn_time = r"time=(\d+)"
ptrn_lockval = r"lockvalue=(\d+)"
ptrn_col = r"col=(\d+)"
ptrn_row = r"row=(\d+)"
ptrn_event = r"event=([A-Z_]+)"


def get_tile(log_line):
    match = re.search(ptrn_col, log_line)
    match1 = re.search(ptrn_row, log_line)
    # print(match,match1)
    # tile = Tile(match[1],match1[1])
    return match[1], match1[1]


def get_lockid(log_line, ptrn):
    match = re.search(ptrn, log_line)
    lockid = 0
    if match:
        lockid = match.group(1)
        # print(f"lockid value: {lockid}")
    return int(lockid)


def get_log_entry(log_line, tile, lockid):
    time = re.search(ptrn_time, log_line)
    event = re.search(ptrn_event, log_line)
    lockval = re.search(ptrn_lockval, log_line)
    if not lockval:
        lockval = "NA"
        le = log_entry(log_line, int(time[1]), event[1], tile, int(lockid), lockval)
    else:
        le = log_entry(
            log_line, int(time[1]), event[1], tile, int(lockid), int(lockval[1])
        )
    return le


def lock_data_extracter(file):
    fo = open(file, "r")
    lockids = []

    main_dict = dict()
    tile_list = []
    for i in fo.readlines():
        # LOCK_ALLOC and LOCK_RELEASE events are checked to extract time
        # information from the trace. DMA_MM2S_START is checked to get the
        # lockids for mm2s locks for differentiation later
        if (
            "event=LOCK_ALLOC" in i
            or "event=LOCK_RELEASE" in i
            or "event=DMA_MM2S_START" in i
        ):

            Col, Row = get_tile(i)

            if "event=LOCK_ALLOC" in i or "event=LOCK_RELEASE" in i:

                if f"Tile({Col},{Row})" not in tile_list:
                    # tile = Tile(Col,Row)
                    tile_list.append(f"Tile({Col},{Row})")
                    # if tile not in main_dict:
                    main_dict[f"Tile({Col},{Row})"] = []
                    # one list for lock id's and another for log_entry object list
                    main_dict[f"Tile({Col},{Row})"].append(list())
                    main_dict[f"Tile({Col},{Row})"].append(list())
                    main_dict[f"Tile({Col},{Row})"].append(list())

                lockid = get_lockid(i, ptrn_lockid_alloc)
                if lockid not in main_dict[f"Tile({Col},{Row})"][0]:
                    main_dict[f"Tile({Col},{Row})"][0].append(lockid)

                log_entry_ = get_log_entry(i, f"Tile({Col},{Row})", lockid)
                main_dict[f"Tile({Col},{Row})"][2].append(log_entry_)

            if "event=DMA_MM2S_START" in i:

                if f"Tile({Col},{Row})" not in tile_list:
                    # tile = Tile(Col,Row)
                    tile_list.append(f"Tile({Col},{Row})")
                    # if tile not in main_dict:
                    main_dict[f"Tile({Col},{Row})"] = []
                    # one list for lock id's and another for log_entry object list
                    main_dict[f"Tile({Col},{Row})"].append(list())
                    main_dict[f"Tile({Col},{Row})"].append(list())
                    main_dict[f"Tile({Col},{Row})"].append(list())

                lockid_mm2s = get_lockid(i, ptrn_lockid_mm2s)
                # Check if lockid_mm2s is not a garbage value. Do not append if its
                # a garbage value. Valid range is less then 15 (kaustubh)
                if (
                    lockid_mm2s not in main_dict[f"Tile({Col},{Row})"][1]
                    and lockid_mm2s < 15
                ):
                    main_dict[f"Tile({Col},{Row})"][1].append(lockid_mm2s)

    fo.close()
    return main_dict


# Extract the read write timing from the extacted trace
def get_rd_wr_time(data):
    # create a dict for storing the timing related data
    timing_dict = dict()
    # To record the start time of transaction
    start_time = 0
    # To record the end time of transaction
    end_time = 0
    # To record the total time of transaction
    exe_time = 0
    # Record the current lockValue to check if its READ or WRITE
    cur_lock_alloc_lockValue = 0

    # Iterate over all the tiles in the log
    for tile in data:
        timing_dict[tile] = []
        timing_dict[tile].append(list())
        print("{} with lock {}".format(tile, data[tile][0]))
        locklist = data[tile][0]

        mm2s_lock_list = data[tile][1]

        # iterate over all the locks in the lock list
        for lock in locklist:
            rd_wr_list = dict()

            if lock not in mm2s_lock_list:
                # print("lock value : {}".format(lock))
                # MOdify the key in the new dict to show the lock read write info
                lock_key = "R_" + str(lock)
                # print("lock key: {}".format(lock_key))
                for log_entry in data[tile][2]:
                    # timing_dict[tile].append()
                    if log_entry.lockid == lock:
                        if lock_key not in rd_wr_list:
                            rd_wr_list[lock_key] = []

                        # print(log_entry)
                        # If LOCK_ALLOC event is present record the start
                        # time if its lock values is READ (0)
                        if log_entry.event == "LOCK_ALLOC":
                            if log_entry.lockValue == 0:  # READ
                                cur_lock_alloc_lockValue = 0
                                start_time = log_entry.time
                            elif log_entry.lockValue == 1:  # WRITE
                                cur_lock_alloc_lockValue = 1
                        if cur_lock_alloc_lockValue == 0:
                            if log_entry.event == "LOCK_RELEASE":
                                if log_entry.lockValue == "NA":
                                    end_time = log_entry.time
                                    exe_time = end_time - start_time
                                    rd_wr_list[lock_key].append(exe_time)
                                    print(
                                        "PL to AIE READ operation for buf {} took {} ps".format(
                                            log_entry.lockid, exe_time
                                        )
                                    )
                timing_dict[tile].append(rd_wr_list)

            elif lock in mm2s_lock_list:
                print("lock value : {}".format(lock))
                lock_key = "W_" + str(lock)
                print("lock key: {}".format(lock_key))
                for log_entry in data[tile][2]:
                    if log_entry.lockid == lock:
                        if lock_key not in rd_wr_list:
                            rd_wr_list[lock_key] = []
                        # print(log_entry)
                        if log_entry.event == "LOCK_ALLOC":
                            if log_entry.lockValue == 1:  # WRITE
                                cur_lock_alloc_lockValue = 1
                                start_time = log_entry.time
                            elif log_entry.lockValue == 0:  # READ
                                cur_lock_alloc_lockValue = 0
                        if cur_lock_alloc_lockValue == 1:
                            if log_entry.event == "LOCK_RELEASE":
                                if log_entry.lockValue == "NA":
                                    end_time = log_entry.time
                                    exe_time = end_time - start_time
                                    rd_wr_list[lock_key].append(exe_time)
                                    print(
                                        "AIE to PL WRITE operation for buf {} took {} ps".format(
                                            log_entry.lockid, exe_time
                                        )
                                    )
                timing_dict[tile].append(rd_wr_list)
    return timing_dict


# A fucntion to print a summary in to CSV
def print_time_summary(time_dict, file_name):
    csvfile = open(file_name, "w", newline="")
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(
        ["Tile", "PL_Read(sum)", "PL_Read(avg)", "PL_Write(sum)", "PL_Write(avg)"]
    )
    for tile in time_dict:
        print(tile)
        sum_over_tile_rd = 0
        avg_over_tile_rd = 0
        sum_over_tile_wr = 0
        avg_over_tile_wr = 0
        no_read_tx = 0
        no_write_tx = 0
        for ele in time_dict[tile]:
            # print(ele)
            if ele != []:
                # Get the sum over iterations
                for i in ele:
                    if ele[i] != []:
                        # Check if the Lock is Read or write
                        if "R" in i:
                            sum_ = sum(ele[i])
                            len_ = len(ele[i])
                            avg = sum_ / len_
                            print(f"Avg over interation for buf{i} : {avg}")
                            sum_over_tile_rd = sum_over_tile_rd + sum_
                            no_read_tx = no_read_tx + len_
                        elif "W" in i:
                            sum_ = sum(ele[i])
                            len_ = len(ele[i])
                            avg = sum_ / len_
                            print(f"Avg over interation for buf{i} : {avg}")
                            sum_over_tile_wr = sum_over_tile_wr + sum_
                            no_write_tx = no_write_tx + len_

        if no_read_tx != 0:
            avg_over_tile_rd = sum_over_tile_rd / no_read_tx
            print(f"Sum over Tile READ opeerations : {sum_over_tile_rd}")
            print(f"Average over Tile READ opeerations : {avg_over_tile_rd}")

        if no_write_tx != 0:
            avg_over_tile_wr = sum_over_tile_wr / no_write_tx
            print(f"Sum over Tile WRITE opeerations : {sum_over_tile_wr}")
            print(f"Average over Tile WRITE opeerations : {avg_over_tile_wr}")
        csvwriter.writerow(
            [
                tile,
                sum_over_tile_rd,
                avg_over_tile_rd,
                sum_over_tile_wr,
                avg_over_tile_wr,
            ]
        )

    csvfile.close()


parser = argparse.ArgumentParser(description="Description of your program")
parser.add_argument("file", help="Path to the input file")
args = parser.parse_args()
file_path = args.file

extracted_data = lock_data_extracter(file_path)
print("Printing extracted Dictionary")
print(extracted_data)
print("==============================")

time_dict = get_rd_wr_time(extracted_data)
print("Printing time_dict Dictionary")
print(time_dict)
print("==============================")

print_time_summary(time_dict, "summary")
