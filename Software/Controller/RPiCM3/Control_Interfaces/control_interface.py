from Software.Controller.RPi3.Control_Interfaces.adc_interface import update
from Software.Controller.RPi3.Control_Interfaces.radio_interface import send, receive

adc_data = []
input_buffer = []
output_buffer = []


def get_data():
    """
    Function for gathering data from all control interfaces
    :return:
    """
    adc_data = update()


def communicate():
    """
    Function for communicating data to user and drone
    :return:
    """
    pass


def control():
    """
    Function that wraps all control interface commands into one function
    :return:
    """
    pass


if __name__ == "__main__":
    control()