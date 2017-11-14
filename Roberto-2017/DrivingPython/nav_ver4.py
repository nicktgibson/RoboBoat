# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 11/9/17


# Only tracks red and drives towards it. As with previous versions, currently is only using a P control


# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2
# from discretePID import PID
import serial
import time

mot = [0, 0, 0, 0]
motMsg = [0, 0, 0, 0]

Drive = False

# initialise motor Control message
motControl = "+000+000+000+000"

# initialise serial port
ser = serial.Serial('/dev/ttyACM0', 9600)  # This one is the Boat
time.sleep(0.5)

# initialise P control
Kp = 1

setPoint = 300
count = 0
countoff = 0

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())

# setting up text
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10, 440)
fontScale = 0.75
fontColor = (0, 0, 255)
lineType = 1

xGreen = 0
yGreen = 0

bottomLeftCornerOfTextRed = (140, 440)

xRed = 0
yRed = 0

# ([0-180]H, [0-255]S, [0-255]V)
# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = (20, 86, 70)
greenUpper = (40, 200, 200)

# lower mask (0-10) for Red
lower_redA = np.array([0, 50, 70])
upper_redA = np.array([7, 255, 200])

# upper mask (170-180) Red
lower_redB = np.array([172, 50, 70])
upper_redB = np.array([180, 255, 200])

ptsRed = deque(maxlen=args["buffer"])
ptsGreen = deque(maxlen=args["buffer"])

# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False):
    camera = cv2.VideoCapture(0)

# otherwise, grab a reference to the video file
else:
    camera = cv2.VideoCapture(args["video"])

# keep looping
while True:

    # flush serial input to avoid crashing when sending serial during loop
    ser.flushInput()

    # grab the current frame
    (grabbed, frame) = camera.read()

    # if we are viewing a video and we did not grab a frame,
    # then we have reached the end of the video
    if args.get("video") and not grabbed:
        break

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

    # join my masks for red
    maskRed = mask0 + mask1

    maskRed = cv2.erode(maskRed, None, iterations=2)
    maskRed = cv2.dilate(maskRed, None, iterations=2)

    # find contours in the mask and initialize the current
    # (x, y) center of the ball
    cntsGreen = cv2.findContours(maskGreen.copy(), cv2.RETR_EXTERNAL,
                                 cv2.CHAIN_APPROX_SIMPLE)[-2]
    cntsRed = cv2.findContours(maskRed.copy(), cv2.RETR_EXTERNAL,
                               cv2.CHAIN_APPROX_SIMPLE)[-2]

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
            cv2.circle(frame, (int(xRed), int(yRed)), int(radiusRed),
                       (0, 255, 255), 2)
            cv2.circle(frame, centerRed, 5, (0, 0, 255), -1)
        else:
            xRed = 0
            yRed = 0

    # update the points queue
    ptsRed.appendleft(centerRed)
    ptsGreen.appendleft(centerGreen)

    # connect circle centers with line
    if int(xRed) > 0 and int(xGreen) > 0:
        cv2.line(frame, (int(xRed), int(yRed)), (int(xGreen), int(yGreen)), (255, 0, 0), 5)
    else:
        cv2.line(frame, (0, 0), (0, 0), (255, 0, 0), 5)

    mX = 0
    mY = 0

    # draw circle at midpoint
    if (xRed != 0) and (xGreen != 0):
        # find midpoint of line
        mX = int(xRed + xGreen) / 2
        mY = int(yRed + yGreen) / 2
        cv2.circle(frame, (mX, mY), 20, (255, 0, 255), -1)

    # showing position
    cv2.putText(frame, "Green Ball",
                (10, 400),
                font,
                fontScale,
                fontColor,
                lineType)
    cv2.putText(frame, str(int(xGreen)) + "," + str(int(yGreen)),
                bottomLeftCornerOfText,
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, "Red Ball",
                (140, 400),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, str(int(xRed)) + "," + str(int(yRed)),
                (140, 440),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, "Center",
                (260, 400),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, str(mX) + "," + str(mY),
                (260, 440),
                font,
                fontScale,
                fontColor,
                lineType)

    """---------------------------------------------"""

    # Driving
    key = cv2.waitKey(1) & 0xFF
    if key == ord("d") or Drive:
        Drive = True

        # Draw boundary lines
        cv2.line(frame, (250, 0), (250, 600), (255, 255, 255), 1)
        cv2.line(frame, (350, 0), (350, 600), (255, 255, 255), 1)
        cv2.line(frame, (300, 0), (300, 600), (255, 0, 255), 3)

        # PID

        # Where I begin to change mX to xRed
        """
        p = (300 - mX) * Kp
        pneg = (300 - mX) * -Kp
        """
        p = (300 - xGreen) * Kp
        pneg = (300 - xGreen) * -Kp


        mot[1] = 255 - abs(p)
        mot[2] = 255 - abs(p)

        mot[0] = -pneg
        mot[3] = pneg

        for i in mot:
            if i > 255:
                i = 255
            elif i < -255:
                i = -255

        for i in range(len(mot)):
            if mot[i] > 99:
                motMsg[i] = '+' + str(int(mot[i]))
            elif mot[i] > 9:
                motMsg[i] = '+0' + str(int(mot[i]))
            elif mot[i] > -1:
                motMsg[i] = '+00' + str(int(mot[i]))
            elif mot[i] > -10:
                motMsg[i] = '-00' + str(int(abs(mot[i])))
            elif mot[i] > -100:
                motMsg[i] = '-0' + str(int(abs(mot[i])))
            elif mot[i] > -256:
                motMsg[i] = str(int(mot[i]))

        motControl = str(motMsg[0]) + str(motMsg[1]) + str(motMsg[2]) + str(motMsg[3]) + '>'

        count += 1
        # Also changed mX to xRed here as well.
        if xGreen != 0 and count >= 10:
            ser.write(motControl)
            count = 0
        elif xGreen == 0:
            countoff += 1

        """
        if countoff >= 25:
            ser.reset_input_buffer()
            ser.write('+000+000+000+000>')
        """

        cv2.putText(frame, motControl,
                    (300, 350),
                    font,
                    fontScale,
                    fontColor,
                    lineType)
        if key == ord("z"):
            ser.write('+000+000+000+000>')
    """-----------------------------------------------------"""

    # show the frame to our screen
    cv2.imshow("Frame", frame)

    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        ser.write('+000+000+000+000>')
        ser.close()
        break

# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()
