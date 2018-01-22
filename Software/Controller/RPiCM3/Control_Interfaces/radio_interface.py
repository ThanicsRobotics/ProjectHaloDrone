import serial
import re

port = serial.Serial(port="/dev/ttyAMA0", baudrate=9600, timeout=1)

coefficients = {"Link Request": 0x01, "Link Acknowledge": 0x02, "Throttle": 0x03, "Roll": 0x04, "Pitch": 0x05,
                "Yaw": 0x06, "X_Velocity": 0x07, "Y_Velocity": 0x08, "Z_Velocity": 0x09, "X_Acceleration": 0x0A,
                "Y_Acceleration": 0x0B, "Z_Acceleration": 0x0C, "Drone_GPS_Latitude": 0x0D, "Drone_GPS_Longitude": 0x0E,
                "Controller_GPS_Latitude": 0x0F, "Controller_GPS_Longitude": 0x10, "Altitude": 0x11, "Fail": 0x12}

input_buffer = ""


def receive():
    """
    Function that reads data from the serial buffer sent from the drone to input_buffer
    Examples of expected data:
        "0x01",
        "0x02",
        "0x04:DATA,0x05:DATA,0x06:DATA,0x07:DATA,0x08:DATA,0x09:DATA,0x0A:DATA,0x0B:DATA,0x0C:DATA,0x0D:DATA,0x0E:DATA,0x11:DATA",
        "0x12"
    """
    global input_buffer

    try:
        read = port.readline()
        input_buffer = read
        print ("Received:", input_buffer)  # Debug

        parse_data()
        verify_data()

        return input_buffer
    except ValueError:
        send(coefficients["Fail"])
        return receive()


def send(val):
    """
    Function that sends data to the drone
    Examples of sendable data:
        "0x01",
        "0x02",
        "0x03:DATA,0x04:DATA,0x05:DATA,0x05:DATA,0x06:DATA,0x0D:DATA,0x0E:DATA,0x11:DATA",
        "0x12"
    """
    port.write((str(val)+"\r\n").encode())
    # print ("Sent:", val)  # Debug


def parse_data():
    parsed = re.split('[:,]', input_buffer)


def verify_data():
    """
    Function that verifies that received data meets standards
    :return: -1 if failed, 0 if verified
    """
    pass
