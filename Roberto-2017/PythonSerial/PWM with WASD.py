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

    if s == 'z': #Stop
        ser.write("+000+000+000+000>")
        time.sleep(1)
        if sLast[0] == '+' #First + or -
            s[0] = '-'
        elif sLast[0]=='-'
            s[0]='+'

        if sLast[4] == '+' #Second + or -
            s[4] = '-'
        elif sLast[4]=='-'
            s[4]='+'

        if sLast[8] == '+' #Third + or -
            s[8] = '-'
        elif sLast[8]=='-'
            s[8]='+'

        if sLast[16] == '+' #Fourth + or -
            s[16] = '-'
        elif sLast[16]=='-'
            s[16]='+'

        time.sleep(500)
        ser.write("+000+000+000+000>")
        ser.close()
        break
    elif s== 'w'
        ser.write("+255+255+255+255>")
        time.sleep(1)
    elif s == 'a'
        ser.write("+255+255-255-255>")
        time.sleep(1)
    elif s == 'd'
        ser.write("-255-255+255+255>")
        time.sleep(1)
    elif s == 'q'
        ser.write("+180+180-180-180>")
        time.sleep(1)
    elif s == 'e'
        ser.write("-180-180+180+180>")
        time.sleep(1)
    else:
        ser.write(s)
        time.sleep(1)
    sLast = s
