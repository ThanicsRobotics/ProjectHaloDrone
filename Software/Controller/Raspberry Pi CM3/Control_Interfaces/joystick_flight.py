from adc_interface import get_pwm
#from radio_interface import send
import time

while True:
    motor_vals = get_pwm()[0:4]
    #print motor_vals  # Debug
    #send(motor_vals)
    time.sleep(.5)
