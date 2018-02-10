port_name = "/dev/ttyACM0"
file_name = "watering.csv"

import serial
import io
import time

s = serial.Serial(port_name)
f = open(file_name,"a");
running = True

while running:
    v = s.readline().decode().strip()
    date = time.strftime("%Y-%m-%d %H:%M:%S")
    f.write(date)
    f.write(",")
    f.write(v)
    f.write("\n")
    f.flush()

    print(v)


