# FSU College of Engineering - AUVSI RoboBoat Competition
# Written by Nicholas Gibson on 2/15/2018

from Tkinter import *
root = Tk()     # Create background window

# Create list
li = 'Carl Patrick Lindsay Helmut Chris Gwen'.split()
listb = Listbox(root)       # create a listbox widget

for item in li:     # Insert each item within li into the listbox
    listb.insert(0,item)

listb.pack()        # Pack listbox
root.mainloop()     # execute the main event handler
