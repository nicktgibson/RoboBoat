# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 10/20/17

# https://stackoverflow.com/questions/29664399/reading-from-two-cameras-in-opencv-at-once

# import the necessary packages
from collections import deque
import numpy as np
import argparse
import imutils
import cv2



"""
To use two camera, switch between the two for each set of frames.
"""





# passing arguments when running code
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())

greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
pts1 = deque(maxlen=args["buffer"])  # length of tail
#pts2 = deque(maxlen=args["buffer"])


# if a video path was not supplied, grab the reference to the webcam
if not args.get("video", False):
    camera1 = cv2.VideoCapture(0)
    #camera1.set(3, 160)
    #camera1.set(4, 120)

    #camera2 = cv2.VideoCapture(1)
    #camera2.set(3, 160)
    #camera2.set(4, 120)


# otherwise, grab a reference to the video file
else:
    camera1 = cv2.VideoCapture(args["video"])
    #camera2 = cv2.VideoCapture(args["video"])

while True:
    # grab the current frame
    (grabbed1, frame1) = camera1.read()
    #(grabbed2, frame2) = camera2.read()

    # if we are viewing a video and we did not grab a frame, then we have reached the end of the video
    # This will need to be fixed later so when both feeds cut out it breaks
    if args.get("video") and not grabbed1:
        break

    frame1 = imutils.resize(frame1, width=600)
    #frame2 = imutils.resize(frame2, width=600)

    blurred1 = cv2.GaussianBlur(frame1, (11, 11), 0)
    #blurred2 = cv2.GaussianBlur(frame2, (11, 11), 0)

    hsv1 = cv2.cvtColor(blurred1, cv2.COLOR_BGR2HSV)
    #hsv2 = cv2.cvtColor(blurred2, cv2.COLOR_BGR2HSV)


    # construct a mask for the color "green", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask

    mask1 = cv2.inRange(hsv1, greenLower, greenUpper)
    mask1 = cv2.erode(mask1, None, iterations=2)
    mask1 = cv2.dilate(mask1, None, iterations=2)

    #mask2 = cv2.inRange(hsv2, greenLower, greenUpper)
    #mask2 = cv2.erode(mask2, None, iterations=2)
    #mask2 = cv2.dilate(mask2, None, iterations=2)

    # find contours in the mask and initialize the current
    # (x, y) center of the ball
    cnts1 = cv2.findContours(mask1.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    #cnts2 = cv2.findContours(mask2.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

    center1 = None
    #center2 = None

    # only proceed if at least one contour was found
    if len(cnts1) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c1 = max(cnts1, key=cv2.contourArea)
        #c2 = max(cnts2, key=cv2.contourArea)

        ((x1, y1), radius1) = cv2.minEnclosingCircle(c1)
        #((x2, y2), radius2) = cv2.minEnclosingCircle(c2)

        M1 = cv2.moments(c1)
        #M2 = cv2.moments(c2)

        center1 = (int(M1["m10"] / M1["m00"]), int(M1["m01"] / M1["m00"]))
        #center2 = (int(M2["m10"] / M2["m00"]), int(M2["m01"] / M2["m00"]))

        # only proceed if the radius meets a minimum size
        if radius1 > 10:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv2.circle(frame1, (int(x1), int(y1)), int(radius1), (0, 255, 255), 2)
            cv2.circle(frame1, center1, 5, (0, 255, 0), -1)
        #if radius2 > 10:
        #    # draw the circle and centroid on the frame,
        #    # then update the list of tracked points
        #    cv2.circle(frame2, (int(x2), int(y2)), int(radius2), (0, 255, 255), 2)
        #    cv2.circle(frame2, center2, 5, (0, 255, 0), -1)

    # update the points queue
    pts1.appendleft(center1)
    #pts2.appendleft(center2)

    # loop over the set of tracked points
    for i in xrange(1, len(pts1)):
        # if either of the tracked points are None, ignore them

        if pts1[i - 1] is None or pts1[i] is None:
            continue

        # otherwise, compute the thickness of the line and draw the connecting lines
        thickness1 = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame1, pts1[i - 1], pts1[i], (0, 255, 0), thickness1)


    #for j in xrange(1, len(pts2)):
    #    # if either of the tracked points are None, ignore them

    #    if pts2[j - 1] is None or pts2[j] is None:
    #        continue

        # otherwise, compute the thickness of the line and draw the connecting lines
    #    thickness2 = int(np.sqrt(args["buffer"] / float(j + 1)) * 2.5)
    #    cv2.line(frame2, pts2[j - 1], pts2[j], (0, 255, 0), thickness2)

    # show the frame to our screen
    cv2.imshow("Frame1", frame1)
    #cv2.imshow("Frame2", frame2)

    key = cv2.waitKey(1) & 0xFF

    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

# cleanup the camera and close any open windows
camera1.release()
#camera2.release()
cv2.destroyAllWindows()
