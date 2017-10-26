import serial
import time


"""
class serial.Serial


__init__(port="/dev/ttyUSB0", baudrate=9600, bytesize=EIGHTBITS, parity=PARITY_NONE, stopbits=STOPBITS_ONE, timeout=None,
         xonxoff=False, rtscts=False, write_timeout=None, dsrdtr=False, inter_byte_timeout=None, exclusive=None)
"""
try:
    ser = serial.Serial('/dev/ttyACM0', 9600)  # This one is the Boat
    time.sleep(0.5)
except:
    ser = serial.Serial('COM28', 9600)   #This one is for windows
    time.sleep(0.5)



print(ser.name)
#r = ser.readline()
#print(r)

while True:
    s = raw_input("Command: ")

    if s == 'z': #Stop
        ser.write("+000+000+000+000>")
        time.sleep(0.025)
        if sLast[0] == '+': # First + or -
            s[0] = '-'
        elif sLast[0]=='-':
            s[0]='+'

        if sLast[4] == '+': #Second + or -
            s[4] = '-'
        elif sLast[4]=='-':
            s[4]='+'

        if sLast[8] == '+': #Third + or -
            s[8] = '-'
        elif sLast[8]=='-':
            s[8]='+'

        if sLast[16] == '+': #Fourth + or -
            s[16] = '-'
        elif sLast[16]=='-':
            s[16]='+'

        time.sleep(500)
        ser.write("+000+000+000+000>")
        ser.close()
        break

    elif s== 'w': #Full speed ahead
        ser.write("+255+255+255+255>")
        time.sleep(0.025)

    elif s== 's': #Full reverse
        ser.write("-255-255-255-255>")
        time.sleep(0.025)

    elif s == 'a': #Full turn port
        ser.write("+255+255-255-255>")
        time.sleep(0.025)

    elif s == 'd': #Full turn starboard
        ser.write("-255-255+255+255>")
        time.sleep(0.025)

    elif s == 'q': #Half turn port
        ser.write("+180+180-180-180>")
        time.sleep(0.025)

    elif s == 'e': #Half turn starboard
        ser.write("-180-180+180+180>")
        time.sleep(0.025)
    else:
        ser.write(s)
        time.sleep(0.025)
    sLast = s
