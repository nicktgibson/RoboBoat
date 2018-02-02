# Written by Nicholas Gibson on 1/31/18

import numpy as np
import serial
import time
import io

# initialise serial port. Won't work for windows see try catch in prior version
ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(0.5)

sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))


while True:
    manual = sio.readline()
    print(manual)
