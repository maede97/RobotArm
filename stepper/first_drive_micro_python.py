# import GPIO
from utime import sleep
from machine import Pin

PORT_EN = 17 # (green)
PORT_DIR = 27 # (red)
PORT_STEP = 22 # (yellow)


pin_en = None
pin_dir = None
pin_step = None

def setup():
    """ we have a single stepper motor, connected at the following ports:
        - PORT_EN (enable pin)
        - PORT_DIR (direction pin)
        - PORT_STEP (step pin)
    """

    # set up the GPIO pins
    pin_en = Pin(PORT_EN, Pin.OUT)
    pin_dir = Pin(PORT_DIR, Pin.OUT)
    pin_step = Pin(PORT_STEP, Pin.OUT)

    # set up the stepper motor
    pin_en.value(1)
    pin_dir.value(0)
    

def step():
    # step the motor
    pin_step.value(1)
    sleep(0.001)
    pin_step.value(0)
    sleep(0.001)


if __name__ == "__main__":
    setup()

    for i in range(10):
        step()

