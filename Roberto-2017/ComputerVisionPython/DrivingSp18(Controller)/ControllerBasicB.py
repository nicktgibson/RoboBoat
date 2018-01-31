# Written by Nicholas Gibson on 1/31/18

import serial
import io

ser = serial.Serial('/dev/ttyACM0')
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
print(ser.name)


with serial.Serial('/dev/ttyACM0', 9600, timeout=10) as ser:
    #x = ser.read()
    #s = ser.read(1)
    sio.flush()
    line = ser.readline()

#print x
#print s
if line == "<Manual Activated>\n":
    print line
    print "Grabbed"
