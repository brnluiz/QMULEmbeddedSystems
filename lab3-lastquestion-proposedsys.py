# This example consider that will be some struct/class defining
# some functions to check some states for buttonColour and buttonSpeed, as:
# - button.isHolded()
# - button.isPressed()
import HWButtons
import HWFunctions

'''
INTER-TASK COMMUNICATION STRUCT
'''
# Settings would save two state variables:
# - speed: SLOW or FAST 
# - colourMode: LED_PER_LED or MULTIPLE_LEDS
# and the funcions to toogle it
class Settings:
    speed = LED_PER_LED;
    colourMode = SLOW
    def speedToogle():
    def colourModeToogle():

'''
TASKS DEFINITION SECTION (3 TASKS)
'''
# Task for the colour sequence toogling button
# Write:
# - settings.colourMode
def buttonColourModeTask():
    if buttonColour.isPressed() && !buttonColour.isHolded():
        # Changes the settings.colourMode variable
        settings.colourModeToggle()

# Task for the speed toogling button
# Write:
# - settings.speed
def buttonSpeedTask():
    if buttonSpeed.isPressed() && !buttonSpeed.isHolded():
        settings.speedToogle()

# Task for the LEDs demonstration
# Read:
# - settings.colourMode to define which function to call
#
# ledPerLedDemo() and multipleLedsDemo()
# Read:
# - settings.speed to blink it faster or slower
def ledDemoTask():
    if settings.colourMode == LED_PER_LED:
        ledPerLedDemo()
    else:
        multipleLedsDemo()

def main():
    # Instantiate a global Settings "class"
    # Would be used to make the tasks communicate with each other
    settings = Settings()

    # Execute the 3 defined tasks
    while(1):
        buttonColourModeTask()
        buttonSpeedTask()
        ledDemoTask()
        delay()

main()