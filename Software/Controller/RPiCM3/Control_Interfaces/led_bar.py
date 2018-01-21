from smbus import SMBus
from collections import deque
import time
from util import map


# =====GLOBAL DEFINITIONS=====


# LED Power Modes
_led_on_full = 0x01
_led_on_half = 0x03
_led_on_quart = 0x07
_led_off = 0x00

# LED Brightnesses
_max = 255
_off = 0

# Addresses
_update_adr = 0x25
_driver_adr = 0x3F
_shutdown_adr = 0x00
_reset_adr = 0x4F

# LED Registers
_leds = {"1r": 0x03, "1g": 0x04, "1b": 0x05, "2r": 0x06, "2g": 0x07, "2b": 0x08,
         "3r": 0x09, "3g": 0x0A, "3b": 0x0B, "4r": 0x0C, "4g": 0x0D, "4b": 0x0E,
         "5r": 0x0F, "5g": 0x10, "5b": 0x11, "6r": 0x12, "6g": 0x13, "6b": 0x14,
         "7r": 0x15, "7g": 0x16, "7b": 0x17, "8r": 0x18, "8g": 0x19, "8b": 0x1A}

_red_leds = [0x03, 0x06, 0x09, 0x0C, 0x0F, 0x12, 0x15, 0x18]
_green_leds = [0x04, 0x07, 0x0A, 0x0D, 0x10, 0x13, 0x16, 0x19]
_blue_leds = [0x05, 0x08, 0x0B, 0x0E, 0x11, 0x14, 0x17, 0x1A]

# Brightness Spectrum
b_spectrum = []
for x in range(0, 256, 4):  # To change rate of brightness change, edit third parameter
    b_spectrum.append(x)

# Color Spectrum variables
c_spectrum = []
c_spectrum_d = deque()
color = [255, 0, 0]

# Bus Creation
bus = SMBus(1)  # Creates new bus on channel 1


# =====COLOR SPECTRUM FUNCTIONS=====


def increase(rate, color_idx):
    """
    Increases the specified color by the given rate
    :param rate: the rate of change
    :param color_idx: the index identifier of the color; 0 (r), 1 (g), or 2 (b)
    """
    while color[color_idx] != 255:
        if color[color_idx] + rate < 255:
            color[color_idx] += rate
        else:
            color[color_idx] = 255
        c_spectrum.append(list(color[:]))


def decrease(rate, color_idx):
    """
    Decreases the specified color by the given rate
    :param rate: the rate of change
    :param color_idx: the index identifier of the color; 0 (r), 1 (g), or 2 (b)
    """
    while color[color_idx] != 0:
        if color[color_idx] - rate > 0:
            color[color_idx] -= rate
        else:
            color[color_idx] = 0
        c_spectrum.append(list(color[:]))


def make_spectrum(rate=8):
    """
    Creates the color spectrum used by spectrum_scroll()
    RECOMMENDED: Call this first in a setup function before scrolling
    :param rate: the rate of spectra divisions
    """
    _rate = int(255 / rate)

    increase(_rate, 1)  # Changes color to YELLOW
    decrease(_rate, 0)  # Changes color to GREEN
    increase(_rate, 2)  # Changes color to CYAN
    decrease(_rate, 1)  # Changes color to BLUE
    increase(_rate, 0)  # Changes color to MAGENTA
    decrease(_rate, 2)  # Changes color to RED


def spectrum_scroll(num_led=8, dir=1):
    """
    Scrolls through entire color spectrum. Is defined by color shifting up by 1 LED
    :param num_led: the number of LEDs through which the transition will flow. Default/max is 8, min is 1
    :param dir: the direction of the scrolling. LEFT is NEGATIVE, RIGHT is POSITIVE
    """
    temp = []
    for l in range(0, 8/num_led):
        temp += list(c_spectrum_d)[l]

    write(temp*num_led)
    c_spectrum_d.rotate(dir)
    time.sleep(0.03125)  # Sets period length (length of rainbow colors * delay)


def fancy_spectrum_scroll(num_led=2):
    temp = []
    for l in range(0, 8 / num_led):
        temp += list(c_spectrum_d)[l]

    write(temp+reversed(temp))
    c_spectrum_d.rotate(dir)
    time.sleep(0.03125)  # Sets period length (length of rainbow colors * delay)


@DeprecationWarning
def long_spectrum_scroll():
    """
    Scrolls through entire color spectrum. Is defined by color shifting through ENTIRE bar
    """
    write(c_spectrum_d[1] * 8)
    c_spectrum_d.rotate()
    time.sleep(0.03125)


@DeprecationWarning
def spectrum_scroll_direction(dir=1):
    """
    Scrolls through spectrum in a given direction.
    NOTE: Speed of scrolling can also be set here, however that is not recommended
    :param dir: the direction of the scrolling. LEFT is NEGATIVE, RIGHT is POSITIVE
    """
    temp = []
    for l in range(0, 8):
        temp += list(c_spectrum_d)[l]

    write(temp)
    c_spectrum_d.rotate(dir)
    time.sleep(0.03125)  # Sets period length (length of rainbow colors * delay)


# =====BRIGHTNESS FUNCTIONS=====


def breathe(led=_leds["1r"], num_led=1, delay=0.1):
    """
    Breathes a set of LEDs based on their BRIGHTNESS
    :param led: the starting LED
    :param num_led: the number of LEDs after the start one that are to be changed as well
    :param delay: the period length of the breathing (length of b_spectrum * delay)
    """
    for brightness in b_spectrum:  # Increase in brightness from off to max
        if num_led != 1:
            write([brightness]*num_led, led)
        else:
            write(brightness, led)
        time.sleep(delay)
    for brightness in reversed(b_spectrum):  # Decrease in brightness from max to off
        if num_led != 1:
            write([brightness]*num_led, led)
        else:
            write(brightness, led)
        time.sleep(delay)


def flash(led=_leds["1r"], val=255, delay_on=0.1, delay_off=0.1):
    """
    Flashes LEDs with given value.
    NOTE: Handles the delay internally, just call in a loop.
    :param led: the LED(s) to flash
    :param val: the brightness of the LED (0-255, default is 255)
    :param delay_on: the time LEDs are on
    :param delay_off: the time LEDs are off
    """
    write(val, led)
    time.sleep(delay_on)
    write(0, led)
    time.sleep(delay_off)


def alternate_flash(led1=_leds["1b"], led2=_leds["1r"], val=255, delay=0.1):
    """
    Flashes alternating colors HARSHLY (straight on/off)
    :param led1: the first LED(s) to flash
    :param led2: the second LED(s) to flash
    :param val: the brightness of the LED(s)
    :param delay: the duty cycle of the LED(s)
    """
    flash(led1, val, delay)
    flash(led2, val, delay)


def alternate_flash_soft(led1=_leds["1b"], led2=_leds["1r"], delay=0.1):
    """
        Flashes alternating colors SOFTLY (breathes on/off)
        :param led1: the first LED(s) to flash
        :param led2: the second LED(s) to flash
        :param delay: the duty cycle of the LED(s)
        """
    breathe(led1, delay=delay)
    breathe(led2, delay=delay)


# =====UTILITY FUNCTIONS=====


def write(data, start_led=_leds["1r"], reg=-1):
    """
    Writes new data to LED driver and automatically updates it. If start_led is defined by a list of LEDs, then it will 
        scroll through the entire list.
    :param data: the data to send to the LED driver. Make sure this data is in a list, even if it is one integer
    :param start_led: the first LED (or LEDs) to interact with
    :param reg: the register to send data to on the LED driver
    """
    if reg == 0x28:  # Executes if writing data to turn on lEDs or adjust current
        bus.write_i2c_block_data(_driver_adr, reg, data)
    elif reg != -1:  # Executes if writing to specific register (used for shutdown, activation, and reset)
        bus.write_byte_data(_driver_adr, reg, data)
    elif type(start_led) is list and type(data) is not list:  # Executes if start_led is a LIST and data is SINGLE
        for led in start_led:
            bus.write_byte_data(_driver_adr, led, data)
    elif type(start_led) is list:  # Executes if start_led is a LIST and data is a LIST
        for led in start_led:
            bus.write_i2c_block_data(_driver_adr, led, data)
    elif type(data) is not list:  # Executes if start_led and data are SINGLE
        bus.write_byte_data(_driver_adr, start_led, data)
    else:  # Executes if start_led is SINGLE and data is a LIST
        bus.write_i2c_block_data(_driver_adr, start_led, data)

    bus.write_byte_data(_driver_adr, _update_adr, 0x00)  # Sends update command to driver


def purge():
    write([0] * 24)


# =====NOTIFICATION FUNCTIONS=====


def boot_notification():
    """
    Executes while controller is booting. By default scrolls through the color SPECTRUM
    NOTE: Need to execute setup() before operation
    """
    print "Booting..."  # Debug
    boot_start = time.time()
    while time.time() <= boot_start + 5:
        spectrum_scroll()
    print "Booted"
    purge()


def link_req_notification():
    """
    Executes when controller is requesting link with drone. By default soft flashes BLUE and RED
    NOTE: Need to execute setup() before operation
    """
    print "Asking for link..."  # Debug
    link_start = time.time()
    while time.time() <= link_start + 5:
        # alternate_flash(_blue_leds, _red_leds, delay=0.125)
        alternate_flash_soft(_blue_leds, _red_leds, delay=0.01)


def success_notification():
    """
    Executes when action (link, maneuver, etc.) is completed successfully. By default hard flashes GREEN
    NOTE: Need to execute setup() before operation
    """
    print "Success!"  # Debug
    ack_start = time.time()
    while time.time() <= ack_start + 1:
        flash(_green_leds, 64, 0.25)


def warning_notification():
    """
    Executes when there is a warning. By default hard flashes YELLOW
    NOTE: A warning is defined by something that requires immediate user attention, but nothing has failed yet
    NOTE: Need to execute setup() before operation
    """
    print "Warning!"  # Debug
    warning_start = time.time()
    while time.time() <= warning_start + 2:
        flash(_red_leds + _green_leds, 64, 0.25)


def failure_notification():
    """
    Executes when there is an error. By default hard flashes RED
    NOTE: A failure is defined as something that has broken operation (e.g. disconnection, battery dead, etc.)
    NOTE: Need to execute setup() before operation
    """
    print "FAILURE!"  # Debug
    fail_start = time.time()
    while time.time() <= fail_start + 2:
        flash(_red_leds, 64, 0.125)


def battery_notification(bat_level):
    """
    Shows battery level in number of LEDs (0-8) on board and colors depending on level (GREEN for >= 50%,
        YELLOW for < 50% but >= 25%, RED for < 25%)
    NOTE: Need to execute setup() before operation
    :param bat_level: the raw voltage of the battery (3.3-4.2)
    """
    bat_led = int(round(map(bat_level, 3.3, 4.2, 0.0, 8.0)))
    # print bat_led  # Debug

    if bat_led > 4:
        leds = _green_leds[0:bat_led]
    elif 2 < bat_led <= 4:
        leds = _red_leds[0:bat_led] + _green_leds[0: bat_led]
    else:
        leds = _red_leds[0:bat_led]

    write(64, leds)


# =====EASTER EGG FUNCTIONS=====


def morse(msg):
    """
    Flashes a message in morse code on the LED bar
    :param msg: the message to be flashes
    """
    print "Flashing:", msg  # Debug

    dot = .1
    dash = 3 * dot
    word_space = 7 * dot

    codes = {"a": ".-", "b": "-...", "c": "-.-.", "d": "-..", "e": ".", "f": "..-.", "g": "--.", "h": "....", "i": "..",
             "j": ".---", "k": "-.-", "l": ".-..", "m": "--", "n": "-.", "o": "---", "p": ".--.", "q": "--.-",
             "r": ".-.", "s": "...", "t": "-", "u": "..-", "v": "...-", "w": ".--", "x": "-..-", "y": "-.--",
             "z": "--..", "0": "-----", "1": ".----", "2": "..---", "3": "...--", "4": "....-", "5": ".....",
             "6": "-....", "7": "--...", "8": "---..", "9": "----."}

    for char in msg:
        if not char.isspace():
            delays = determine_delays(codes[char.lower()], dot, dash)
            for unit in delays:
                flash(_blue_leds, val=64, delay_on=unit, delay_off=dot)
                time.sleep(dash)
        else:
            time.sleep(word_space)


def determine_delays(code, dot, dash):
    """
    Determines the delays necessary for the letter being transmitted
    :param code: the code for the letter
    :param dot: the dot unit length
    :param dash: the dash unit length
    :return: the delay code for the letter
    """
    delays = []
    for char in code:
        if char == ".":
            delays.append(dot)
        else:
            delays.append(dash)
    return delays


# =====IMPLEMENTATION FUNCTIONS=====


def setup():
    """
    Sets up LED driver with proper power settings and initializes any global variable necessary for operation
    """
    global c_spectrum_d

    # Turn on LEDs
    write(0x01, reg=_shutdown_adr)

    # Activate LEDs at set current
    led_block = []
    for i in range(0, 25):
        led_block.append(_led_on_quart)  # To change current through LEDs edit the appended code
    write(led_block, reg=0x28)  # NOTE: 0x28 is the setup code for LED 1R

    # Create color spectrum
    make_spectrum()
    c_spectrum_d = deque(c_spectrum)


def test():
    """
    Basic testing to ensure functionality of any operation.
    NOTE: To operate, just call - the while loop and setup() is implemented natively
    """
    try:
        setup()
        while True:
            # spectrum_scroll()
            # long_spectrum_scroll()
            # spectrum_scroll_direction(-1)
            # write(255, [_leds["1b"], _leds["2b"], _leds["3b"], _leds["4b"]])
            # breathe(_blue_leds, delay=0.05)
            # flash()
            # alternate_flash(_blue_leds, _red_leds, delay=0.5)
            morse("Hello World")
    except KeyboardInterrupt:
        print "Exiting..."
    finally:
        print "Cleaning..."
        clean()


def demo():
    """
    Demonstration to show off functionality
    NOTE: To operate, place command in a while loop
    """
    setup()

    try:
        boot_notification()

        link_req_notification()

        success_notification()

        warning_notification()

        failure_notification()

        i = 3.3
        while i < 4.2:
            battery_notification(i)
            # print i  # Debug
            time.sleep(2)
            purge()
            i += .1

        clean()
    except KeyboardInterrupt:
        print "Exiting..."
    finally:
        print "Cleaning..."
        clean()


def clean():
    """
    Cleans up LED driver to ensure smooth transition between operations
    :return:
    """
    bus.write_byte_data(_driver_adr, _shutdown_adr, 0x00)  # Shuts down LEDs
    bus.write_byte_data(_driver_adr, _reset_adr, 0x4F)  # Resets LED registers
