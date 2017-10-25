# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 10/23/17

# import the necessary packages

from imutils.perspective import four_point_transform
from imutils import contours
import imutils
import cv2

# define the dictionary of digit segments so we can identify
# each digit on the thermostat

DIGITS_LOOKUP = {
    (1, 1, 1, 0, 1, 1, 1): 0,
    (0, 0, 1, 0, 0, 1, 0): 1,
    (1, 0, 1, 1, 1, 1, 0): 2,
    (1, 0, 1, 1, 0, 1, 1): 3,
    (0, 1, 1, 1, 0, 1, 0): 4,
    (1, 1, 0, 1, 0, 1, 1): 5,
    (1, 1, 0, 1, 1, 1, 1): 6,
    (1, 0, 1, 0, 0, 1, 0): 7,
    (1, 1, 1, 1, 1, 1, 1): 8,
    (1, 1, 1, 1, 0, 1, 1): 9
}


camera = cv2.VideoCapture(0)


while True:
    # Grab is true or false, frame is the actual frame
    (grabbed, frame) = camera.read()

    # Resize frame
    frame = imutils.resize(frame, width=500)

    # convert frame to grayscale and rename to gray
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # blur it and rename
    blurred = cv2.GaussianBlur(gray, (5,5), 0)

    # create edge map and rename
    edged = cv2.Canny(blurred, 50 , 200, 255)












    key = cv2.waitKey(1) & 0xFF
    cv2.imshow("Frame", frame)

    if key == ord("q"):
        break

camera.release()
cv2.destroyAllWindows()