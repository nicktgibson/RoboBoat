# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 12/13/2017

import matplotlib.pyplot as plt
import numpy as np


# Example plotting
# plt.plot([1, 2, 3, 4])
# plt.ylabel('some numbers')
# plt.show()

# Example matrix
# n = np.matrix([[1, 2], [3, 4]])
# n.item(0,0) is 1

# sampleData is a matrix of fake readings from the LIDAR and GPS
# angles are measured with standard CCW motion
# assume max sensor range is 10
# assume objects are radius 0.5, must grow to account for where they might be
sampleData = np.matrix([
    ['GPSx', 'GPSy', 'reading0', 'reading45', 'reading90', 'reading135', 'reading180'],
    [1, 1, 1, 1, 1, 1, 1]
])

plt.plot(sampleData.item(1,0), sampleData.item(1,1), 'go')
plt.axis([0, 20, 0, 20])
plt.show()

plt.plot(2, 2, 'rx')
plt.draw()
