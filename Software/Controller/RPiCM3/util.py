import json
import os

import requests

os.chdir("RPiCM3")
loc = os.path.abspath("Settings.json")
with open(loc) as settings_file:
    settings = json.load(settings_file)
os.chdir("GUI_V3")


class Colors:
    thanics_blue = None
    white = None
    black = None
    green = None
    red = None

    def __init__(self):
        self.thanics_blue = self.convert_rgb_values(settings["general"]["colors"]["thanics_blue"])
        # print misc_color  # Debug
        self.white = self.convert_rgb_values(settings["general"]["colors"]["white"])
        # print bg_color  # Debug
        self.black = self.convert_rgb_values(settings["general"]["colors"]["black"])
        # print txt_color  # Debug
        self.green = self.convert_rgb_values(settings["general"]["colors"]["green"])
        # print true_color  # Debug
        self.red = self.convert_rgb_values(settings["general"]["colors"]["red"])
        # print false_color  # Debug

    def convert_rgb_values(self, val):
        con_val = []
        count = 0
        while count != 3:
            x = val[count]
            x /= 255
            con_val.append(x)
            # print con_val # Debug
            count += 1
        con_val.append(val[3])
        return con_val


def map(x, in_min, in_max, out_min, out_max):
    """
    Function that converts a value between two different scales
    :param x: The value to be converted
    :param in_min: the minimum of the input range
    :param in_max: the maximum of the input range
    :param out_min: the minimum of the output range
    :param out_max: the maximum of the output range
    :return: the value converted to the output range.
    """
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


def get_controller_geo_ip():
    send_url = 'http://freegeoip.net/json'
    r = requests.get(send_url)
    j = json.loads(r.text)
    return j['latitude'], j['longitude']
