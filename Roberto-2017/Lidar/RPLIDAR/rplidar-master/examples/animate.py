#!/usr/bin/env python
'''Animates distances and measurment quality'''
from rplidar import RPLidar
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.animation as animation
import os
import sys

print("Need sudo permission to give USB correct permissions")
os.system('sudo chmod 666 /dev/ttyUSB0')    # Gives usb permission to write

PORT_NAME = '/dev/ttyUSB0'
DMAX = 1000    # Changed from original 500 to better show full range of scanner. 15000
IMIN = 0
IMAX = 50

def update_line(num, iterator, line):
    scan = next(iterator)
    offsets = np.array([(np.radians(meas[1]), meas[2]) for meas in scan])
    line.set_offsets(offsets)
    intens = np.array([meas[0] for meas in scan])
    line.set_array(intens)
    return line,

def run():
    try:
        lidar = RPLidar(PORT_NAME)
    except:
        print("Connection Error: Check USB")
        sys.exit()
    fig = plt.figure()
    ax = plt.subplot(111, projection='polar')
    line = ax.scatter([0, 0], [0, 0], s=5, c=[IMIN, IMAX],
                           cmap=plt.cm.Greys_r, lw=0)
    ax.set_rmax(DMAX)
    ax.grid(True)

    iterator = lidar.iter_scans()
    ani = animation.FuncAnimation(fig, update_line,
        fargs=(iterator, line), interval=50)
    plt.show()
    lidar.stop()
    lidar.stop_motor()
    lidar.disconnect()

if __name__ == '__main__':
    run()
