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
    print(adc_interface.get_calibrated_pwm())
    yaw = adc_interface.calibrated_pwm[0]
    altitude = adc_interface.calibrated_pwm[1]
    roll = adc_interface.calibrated_pwm[2]
    pitch = adc_interface.calibrated_pwm[3]
    send(3)
    send(altitude)
    send(4)
    send(roll)
    send(5)
    send(pitch)
    send(6)
    send(yaw)
    time.sleep(.003)
    print("Roll: "+roll.encode()+" | Pitch: "+pitch.encode()+" | Yaw: "+yaw.encode()+" | Altitude: "+altitude.encode())
