# import GPIO
from time import sleep
import RPi.GPIO as GPIO

PORT_EN = 17 # (green)
PORT_DIR = 27 # (red)
PORT_STEP = 22 # (yellow)

def setup():
    """ we have a single stepper motor, connected at the following ports:
        - PORT_EN (enable pin)
        - PORT_DIR (direction pin)
        - PORT_STEP (step pin)
    """

    # set up the GPIO pins
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(PORT_EN, GPIO.OUT)
    GPIO.setup(PORT_DIR, GPIO.OUT)
    GPIO.setup(PORT_STEP, GPIO.OUT)

    # set up the stepper motor
    GPIO.output(PORT_EN, GPIO.HIGH)
    GPIO.output(PORT_DIR, GPIO.LOW)

def step():
    # step the motor
    GPIO.output(PORT_STEP, GPIO.HIGH)
    sleep(0.001)
    GPIO.output(PORT_STEP, GPIO.LOW)
    sleep(0.001)

def cleanup():
    # clean up the GPIO pins
    GPIO.cleanup()


if __name__ == "__main__":
    try:
        setup()

        for i in range(10):
            step()
    finally:
        cleanup()