# Written by Nicholas Gibson on 2/1/18

import pygame
import numpy as np
import serial
import time

# Initialise motor Control message
motControl = "+000+000+000+000"

# Initialise serial port
#ser = serial.Serial('/dev/ttyACM0', 9600) # For the boat
time.sleep(0.5)

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def Print(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10

    def blankline(self):
        self.y += self.line_height


pygame.init()

# Set the width and height of the screen [width,height]
size = [500, 300]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("My Game")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()

# Get ready to print
textPrint = TextPrint()

# -------- Main Program Loop -----------
while done == False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop

        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
        if event.type == pygame.JOYBUTTONUP:
            print("Joystick button released.")

    # DRAWING STEP
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(WHITE)
    textPrint.reset()

    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()

    #textPrint.Print(screen, "Number of joysticks: {}".format(joystick_count))
    #textPrint.indent()

    joystick = pygame.joystick.Joystick(0)
    joystick.init()

    textPrint.Print(screen, "RoboBoat Wireless Controller: Manual Mode".format(0))
    name = joystick.get_name()
    textPrint.Print(screen, "Joystick name: {}".format(name))

    textPrint.indent()

    axisLY = joystick.get_axis(1)
    axisRY = joystick.get_axis(4)

    textPrint.Print(screen, "Axis LY value: {:>6.3f}".format(axisLY))
    textPrint.Print(screen, "Axis RY value: {:>6.3f}".format(axisRY))
    textPrint.blankline()

    buttonB = joystick.get_button(1)
    textPrint.Print(screen, "Emergency Stop: {}".format(buttonB))

    textPrint.blankline()

    leftMot = int(axisLY*-255)
    rightMot = int(axisRY*-255)

    textPrint.Print(screen, "Motor 1: {}".format(leftMot))

    textPrint.Print(screen, "Motor 2: {}".format(leftMot))

    textPrint.Print(screen, "Motor 3: {}".format(rightMot))

    textPrint.Print(screen, "Motor 4: {}".format(rightMot))

    # Making motor message

    """
    # Left Motor sign
    if np.sign(leftMot) == -1:
        motControl = '-' + motControl[1:16]
        motControl = motControl[0:4] + '-' + motControl[5:16]
    else:
        motControl = '+' + motControl[1:16]
        motControl = motControl[0:4] + '+' + motControl[5:16]

    # Right Motor sign
    if np.sign(rightMot) == -1:
        motControl = motControl[0:8] + '-' + motControl[9:16]
        motControl = motControl[0:12] + '-' + motControl[13:16]
    else:
        motControl = motControl[0:8] + '+' + motControl[9:16]
        motControl = motControl[0:12] + '+' + motControl[13:16]

    # Left Motor value
    
    if leftMot > 255:
        leftMot = 255
    elif leftMot < -255:
        leftMot = -255
    """

    if leftMot > 99:
        leftMot = '+' + str(int(leftMot))
    elif leftMot > 9:
        leftMot = '+0' + str(int(leftMot))
    elif leftMot > -1:
        leftMot = '+00' + str(int(leftMot))
    elif leftMot > -10:
        leftMot = '-00' + str(int(abs(leftMot)))
    elif leftMot > -100:
        leftMot = '-0' + str(int(abs(leftMot)))
    elif leftMot > -256:
        leftMot = str(int(leftMot))

    # Right mot value
    if rightMot > 99:
        rightMot = '+' + str(int(rightMot))
    elif rightMot > 9:
        rightMot = '+0' + str(int(rightMot))
    elif rightMot > -1:
        rightMot = '+00' + str(int(rightMot))
    elif rightMot > -10:
        rightMot = '-00' + str(int(abs(rightMot)))
    elif rightMot > -100:
        rightMot = '-0' + str(int(abs(rightMot)))
    elif rightMot > -256:
        rightMot = str(int(rightMot))

    motControl = '<' + leftMot + leftMot + rightMot + rightMot + '>'

    textPrint.blankline()
    textPrint.Print(screen, "{}".format(motControl))

    # Display Screen
    pygame.display.flip()

    # Limit to 20 frames per second
    clock.tick(20)

# Close the window and quit.
pygame.quit()
