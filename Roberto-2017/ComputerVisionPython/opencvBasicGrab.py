# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 10/25/17

# drawing on frame
# https://docs.opencv.org/3.1.0/dc/da5/tutorial_py_drawing_functions.html

import cv2
import imutils

camera = cv2.VideoCapture(0)

while True:
    # Grab is true or false, frame is the actual frame
    (grabbed, frame) = camera.read()

    # Resize frame
    frame = imutils.resize(frame, width=800)



    key = cv2.waitKey(1) & 0xFF
    cv2.imshow("Frame", frame)

    # press q to stop
    if key == ord("q"):
        break

camera.release()
cv2.destroyAllWindows()