# import GPIO
import RPi.GPIO as GPIO

PORT_EN = 17
PORT_DIR = 27
PORT_STEP = 22

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
    GPIO.output(PORT_STEP, GPIO.LOW)

def cleanup():
    # clean up the GPIO pins
    GPIO.cleanup()


if __name__ == "__main__":
    setup()

    for i in range(10):
        step()
    
    cleanup()