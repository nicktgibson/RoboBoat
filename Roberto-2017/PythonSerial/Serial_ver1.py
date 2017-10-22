import serial
import time


"""
class serial.Serial


__init__(port="/dev/ttyUSB0", baudrate=9600, bytesize=EIGHTBITS, parity=PARITY_NONE, stopbits=STOPBITS_ONE, timeout=None,
         xonxoff=False, rtscts=False, write_timeout=None, dsrdtr=False, inter_byte_timeout=None, exclusive=None)
"""
ser = serial.Serial('/dev/ttyACM0', 9600)  # open serial port
print(ser.name)
#r = ser.readline()
#print(r)

while True:

    s = raw_input("Command: ")

    ser.write(s)
    if s == 'z':
        ser.write("+000+000+000+000>")
        time.sleep(1)
        ser.close()
        break

