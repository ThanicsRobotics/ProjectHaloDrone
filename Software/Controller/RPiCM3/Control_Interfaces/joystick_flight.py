from .adc_interface import get_pwm
from .radio_interface import send
import time

while True:
    motor_vals = get_pwm()[0:4]
    #print motor_vals  # Debug
    send(3)
    send(motor_vals[1])
    send(4)
    send(motor_vals[2])
    send(5)
    send(motor_vals[3])
    send(6)
    send(motor_vals[0])
    time.sleep(.5)
