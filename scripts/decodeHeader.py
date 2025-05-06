import os 
import sys

input =  sys.argv[1]

print(input)
print(bin(int(input)))
len_ip = len(bin(int(input)))

packet_id = 0
packet_id_s = len_ip
packet_id_e = len_ip - 4 - 1

packet_type = 0 
packet_type_s = len_ip - 12
packet_type_e = len_ip - 14 -1

source_row = 0 
source_row_s = len_ip - 16
source_row_e = len_ip - 20 -1

source_col = 0 
source_col_s = len_ip - 21
source_col_e = len_ip - 27 -1

packet_id = bin(int(input))[packet_id_e:packet_id_s]
print(packet_id)
print("Packet ID " , int(packet_id,2))

packet_type = bin(int(input))[packet_type_e:packet_type_s]
print("Packet Type " , int(packet_type,2))

source_row = bin(int(input))[source_row_e:source_row_s]
print("source_row " , int(source_row,2))

source_col = bin(int(input))[source_col_e:source_col_s]
print("source_col " , int(source_col,2))
