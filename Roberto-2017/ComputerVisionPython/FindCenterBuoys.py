# Written by Nicholas Gibson on 4/10/18

# Written by Nicholas Gibson on 2/14/18

# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2

# setting up text
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10, 440)
fontScale = 0.75
fontColor = (0, 0, 255)
lineType = 1

bottomLeftCornerOfTextRed = (140, 440)

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())

# ([0-180]H, [0-255]S, [0-255]V)
# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
#greenLower = (20, 86, 70)
#greenUpper = (40, 200, 200)

#green = (45, 180, 175)
green = (156, 170, 46) # BGR
#print(cv2.cvtColor((green), cv2.COLOR_BGR2HSV))

greenLower = (35, 160, 100)
greenUpper = (50, 200, 255)

red = (71, 70, 190)

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

fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('boatvideo5.avi', fourcc, 20.0, (640, 480))

xGreen = 0
yGreen = 0
xRed = 0
yRed = 0
i = 0;

# keep looping
while True:
    # grab the current frame
    (grabbed, frame) = camera.read()

    # if we are viewing a video and we did not grab a frame,
    # then we have reached the end of the video
    if args.get("video") and not grabbed:
        break

    # resize the frame, blur it, and convert it to the HSV color space
    frame = imutils.resize(frame, width=640, height=480)

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
                (10, 280),
                font,
                fontScale,
                fontColor,
                lineType)
    cv2.putText(frame, str(int(xGreen)) + "," + str(int(yGreen)),
                (10, 320),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, "Red Ball",
                (140, 280),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, str(int(xRed)) + "," + str(int(yRed)),
                (140, 320),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, "Center",
                (260, 280),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.putText(frame, str(mX) + "," + str(mY),
                (260, 320),
                font,
                fontScale,
                fontColor,
                lineType)

    cv2.circle(frame, (200, 230), 20, green, thickness=-1)
    cv2.circle(frame, (79, 230), 20, red, thickness=-1)


    # update the points queue
    ptsRed.appendleft(centerRed)
    ptsGreen.appendleft(centerGreen)

    """ This block draws the history lines. """

    # loop over the set of tracked points
    for i in xrange(1, len(ptsGreen)):
        # if either of the tracked points are None, ignore
        # them
        if ptsGreen[i - 1] is None or ptsGreen[i] is None:
            continue

        # otherwise, compute the thickness of the line and
        # draw the connecting lines
        thicknessGreen = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame, ptsGreen[i - 1], ptsGreen[i], (0, 255, 0), thicknessGreen)

    for i in xrange(1, len(ptsRed)):
        # if either of the tracked points are None, ignore
        # them
        if ptsRed[i - 1] is None or ptsRed[i] is None:
            continue

        # otherwise, compute the thickness of the line and
        # draw the connecting lines
        thicknessRed = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame, ptsRed[i - 1], ptsRed[i], (0, 0, 255), thicknessRed)

    # show the frame to our screen
    cv2.imshow("Frame", frame)

    out.write(frame)
    cv2.imwrite('roboBoat'+str(i)+'.jpg', frame)

    i += 1

    key = cv2.waitKey(1) & 0xFF
    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

# cleanup the camera and close any open windows
camera.release()
out.release()
cv2.destroyAllWindows()