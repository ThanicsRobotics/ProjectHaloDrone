from RPiCM3.Control_Interfaces import adc_interface
from .radio_interface import send
import time

def calibrateJoystickRange():
    motor_vals = adc_interface.get_pwm()
    print("Center joysticks within 3 seconds\r\n")
    time.sleep(3)
    
    adc_interface.yawCenter = motor_vals[0]
    # print(adc_interface.yawCenter)
    adc_interface.altitudeCenter = motor_vals[1]
    # print(adc_interface.altitudeCenter)
    adc_interface.rollCenter = motor_vals[2]
    # print(adc_interface.rollCenter)
    adc_interface.pitchCenter = motor_vals[3]
    # print(adc_interface.pitchCenter)

adc_interface.yawCenter = 1503
adc_interface.altitudeCenter = 1560
adc_interface.rollCenter = 1539
adc_interface.pitchCenter = 1524

# calibrateJoystickRange()
while True:
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
    time.sleep(.003)
