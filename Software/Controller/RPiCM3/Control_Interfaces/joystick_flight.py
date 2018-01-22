import adc_interface
from .radio_interface import send
import time

while True:
    calibrateJoystickRange()
    adc_interface.get_calibrated_pwm()
    #print motor_vals  # Debug
    send(3)
    send(adc_interface.calibrated_pwm[1])
    send(4)
    send(adc_interface.calibrated_pwm[2])
    send(5)
    send(adc_interface.calibrated_pwm[3])
    send(6)
    send(adc_interface.calibrated_pwm[0])
    time.sleep(.5)

def calibrateJoystickRange():
    motor_vals = adc_interface.get_pwm()
    print("Center joysticks within 3 seconds\r\n")
    time.sleep(3)
    
    adc_interface.yawOffset = 1500 - motor_vals[0]
    adc_interface.altitudeOffset = 1500 - motor_vals[1]
    adc_interface.rollOffset = 1500 - motor_vals[2]
    adc_interface.pitchOffset = 1500 - motor_vals[3]
