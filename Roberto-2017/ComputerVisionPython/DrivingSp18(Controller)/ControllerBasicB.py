# Written by Nicholas Gibson on 1/31/18

import serial
import io

ser = serial.Serial('/dev/ttyACM0')
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
print(ser.name)


ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2)
#x = ser.read()
#s = ser.read(1)


#print x
#print s
line = "string"
while(line[0:18] != "<Manual Activated>"):
    sio.flush()
    line = ser.readline()
    print line[0:18]
    print line

ser.close()
