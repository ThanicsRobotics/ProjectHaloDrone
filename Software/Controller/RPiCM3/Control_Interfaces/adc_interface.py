from smbus import SMBus
from ..util import map

# =====VARIABLE DEFINITIONS=====

adr = 0x33
config = 0x0B  # Reads values from AIN0-AIN5

values_raw = []
values_volt = []
values_pwm = []

bus = SMBus(1)

global yawCenter
global pitchCenter
global rollCenter
global altitudeCenter

# =====CRITICAL FUNCTIONS=====


def parse(data):
    """
    Function that parses raw adc values to readable data
    :param data: raw adc values
    """
    global values_raw

    values_raw = []

    for x in range(0, 12):
        if x % 2 == 0:
            data[x] = data[x] - 252  # Subtracts 252 from the first byte of data to get rid of 111111 sent by ADC
    # print data  # Debug

    # Combines the two bytes together into one usable 10-bit value
    for x in range(0, 11, 2):
        values_raw.append(bin(data[x]).lstrip('-0b').zfill(2) + bin(data[x + 1]).lstrip('-0b').zfill(8))
        # print values  # Debug


def parse_to_volts(data):
    """
    Function that converts raw adc values into voltage values
    (Note: Updates values_raw and values_volts)
    :param data: raw adc values
    """
    global values_volt

    values_volt = []

    parse(data)

    c = 0
    for byte in values_raw:
        if c == 0:
            values_volt.append(to_voltage(int(byte, 2) * 2))
            c += 1
        else:
            values_volt.append(to_voltage(int(byte, 2)))
    # print values_volt  #Debug


def parse_to_pwm(data):
    """
    Function that converts raw adc values for Joy1 and Joy2 into usable PWM values.
    (Note: Updates values_raw, values_volts, and values_pwm)
    :param data: raw adc values
    """
    global values_pwm

    values_pwm = []

    parse_to_volts(data)

    val_needed = values_volt[1:5]
    for val in val_needed:
        values_pwm.append(map(val, 0, 3.3, 1000, 2000))
    #print (values_pwm)  # Debug


# =====UTILITY FUNCTIONS=====


def update():
    """
    Gets the raw adc values and updates/parses the value arrays
    """
    data = bus.read_i2c_block_data(adr, config, 12)
    parse_to_pwm(data)
    return values_pwm


def to_voltage(val):
    """
    Function that converts a value from a raw value to voltage
    :param val: the raw adc value
    :return: the converted value in volts
    """
    return (val / 1024.0) * 3.3


def get_raw():
    """
    Function that wraps updating the adc values (raw and converted) and returns the raw data
    (Note: Does not sent setup byte, that is done in __init__.py
    :return: The raw values parsed from the adc values
    """
    update()
    return values_raw


def get_volts():
    """
    Function that wraps updating the adc values (raw and converted) and returns the raw data
    (Note: Does not sent setup byte, that is done in __init__.py
    :return: The voltage values parsed from the adc values
    """
    update()
    return values_volt


def get_pwm():
    """
    Function that wraps updating the adc values (raw and converted) and returns the raw data
    (Note: Does not sent setup byte, that is done in __init__.py
    :return: The pwm values parsed from the adc values
    """
    update()
    return values_pwm

def get_calibrated_pwm():
    global calibrated_pwm
    update()
    calibrated_pwm = [0,0,0,0]

    calibrated_pwm[0] = map(values_pwm[0], 1000, yawCenter, 1000, 1500) if values_pwm[0] < yawCenter else map(values_pwm[0], yawCenter, 2000, 1500, 2000)
    calibrated_pwm[1] = map(values_pwm[1], 1000, altitudeCenter, 1000, 1500) if values_pwm[1] < altitudeCenter else map(values_pwm[1], altitudeCenter, 2000, 1500, 2000)
    calibrated_pwm[2] = map(values_pwm[2], 1000, rollCenter, 1000, 1500) if values_pwm[2] < rollCenter else map(values_pwm[2], rollCenter, 2000, 1500, 2000)
    calibrated_pwm[3] = map(values_pwm[3], 1000, pitchCenter, 1000, 1500) if values_pwm[3] < pitchCenter else map(values_pwm[3], pitchCenter, 2000, 1500, 2000)
    return calibrated_pwm
# =====EXAMPLE=====


# bus.write_byte(adr, setup)
# while True:
#     data = bus.read_i2c_block_data(adr, config, 12)
#     values_volt = parse(data)
#
#     for val in values_volt:
#         values_pwm.append(map(val, 0, 3.3, 1000, 2000))
#         # print values_pwm  # Debug
#     print values_pwm  # Debug
#
#     time.sleep(.125)
