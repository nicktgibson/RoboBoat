# Written by Nicholas Gibson on 2/22/18

# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())

# ([0-180]H, [0-255]S, [0-255]V)
# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = np.array([70, 0, 0])
greenUpper = np.array([90, 255, 255])

# lower mask (0-10) for Red
lower_redA = np.array([0, 50, 70])
upper_redA = np.array([15, 255, 200])


# upper mask (170-180) Red
lower_redB = np.array([165, 50, 70])
upper_redB = np.array([180, 255, 255])

ptsRed = deque(maxlen=args["buffer"])
ptsGreen = deque(maxlen=args["buffer"])

frame = cv2.imread('images/buoyBoth.jpeg')


# resize the frame, blur it, and convert it to the HSV color space
frame = imutils.resize(frame, width=600)

# blurred = cv2.GaussianBlur(frame, (11, 11), 0)
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

# construct a mask for the color "green", then perform
# a series of dilations and erosions to remove any small
# blobs left in the mask
maskGreen = cv2.inRange(hsv, greenLower, greenUpper)
maskGreen = cv2.erode(maskGreen, None, iterations=2)
maskGreen = cv2.dilate(maskGreen, None, iterations=2)

mask0 = cv2.inRange(hsv, lower_redA, upper_redA)
mask1 = cv2.inRange(hsv, lower_redA, upper_redA)

# join my masks
maskRed = mask0 + mask1

maskRed = cv2.erode(maskRed, None, iterations=2)
maskRed = cv2.dilate(maskRed, None, iterations=2)

# find contours in the mask and initialize the current
# (x, y) center of the ball
cntsGreen = cv2.findContours(maskGreen.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[-2]

cntsRed = cv2.findContours(maskRed.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

centerRed = None
centerGreen = None

# only proceed if at least one contour was found
if len(cntsGreen) > 0:
    # find the largest contour in the mask, then use
    # it to compute the minimum enclosing circle and
    # centroid
    cGreen = max(cntsGreen, key=cv2.contourArea)
    ((xGreen, yGreen), radiusGreen) = cv2.minEnclosingCircle(cGreen)
    MGreen = cv2.moments(cGreen)
    centerGreen = (int(MGreen["m10"] / MGreen["m00"]), int(MGreen["m01"] / MGreen["m00"]))

    # only proceed if the radius meets a minimum size
    if radiusGreen > 10:
        # draw the circle and centroid on the frame,
        # then update the list of tracked points
        cv2.circle(frame, (int(xGreen), int(yGreen)), int(radiusGreen),
                   (0, 255, 255), 2)
        cv2.circle(frame, centerGreen, 5, (0, 255, 0), -1)
    else:
        xGreen = 0
        yGreen = 0

# only proceed if at least one contour was found
if len(cntsRed) > 0:
    # find the largest contour in the mask, then use
    # it to compute the minimum enclosing circle and
    # centroid
    cRed = max(cntsRed, key=cv2.contourArea)
    ((xRed, yRed), radiusRed) = cv2.minEnclosingCircle(cRed)
    MRed = cv2.moments(cRed)
    centerRed = (int(MRed["m10"] / MRed["m00"]), int(MRed["m01"] / MRed["m00"]))

    # only proceed if the radius meets a minimum size
    if radiusRed > 10:
        # draw the circle and centroid on the frame,
        # then update the list of tracked points
        cv2.circle(frame, (int(xRed), int(yRed)), int(radiusRed), (0, 255, 255), 2)
        cv2.circle(frame, centerRed, 5, (0, 0, 255), -1)
    else:
        xRed = 0
        yRed = 0

# show the frame to our screen
cv2.imshow("Frame", frame)
cv2.imwrite("NewImage.jpg", frame)

