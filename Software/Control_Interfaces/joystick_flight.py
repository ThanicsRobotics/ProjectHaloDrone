from adc_interface import get_pwm
from radio_interface import send

while True:
    motor_vals = get_pwm()[0, 3]
    print motor_vals  # Debug
    send(motor_vals)

