import os

from kivy.properties import StringProperty
from kivy.uix.actionbar import ActionBar, ActionPrevious
from kivy.uix.gridlayout import GridLayout


class Header(GridLayout):
    top_left = os.path.abspath("icons/CockpitTL.png")
    drone_gps = StringProperty(os.path.abspath("icons/GPSIcon.png"))
    drone_battery = StringProperty(os.path.abspath("icons/BatteryFull.png"))
    drone_connection = StringProperty(os.path.abspath("icons/DroneNo.png"))

    top_right = os.path.abspath("icons/CockpitTR.png")
    controller_gps = StringProperty(os.path.abspath("icons/GPSIcon.png"))
    wifi_connection = StringProperty(os.path.abspath("icons/WiFiFull.png"))
    controller_battery = StringProperty(os.path.abspath("icons/Battery25.png"))

    def update_drone_gps(self):
        # TODO: Implement drone GPS strength
        self.drone_gps = os.path.abspath("icons/MissingTexture.png")

    def update_drone_battery(self):
        # TODO: Implement drone battery strength
        self.drone_battery = ""

    def update_drone_connection(self):
        # TODO: Implement drone connection strength
        self.drone_connection = ""

    def update_controller_gps(self):
        # TODO: Implement controller gps strength
        self.controller_gps = ""

    def update_wifi(self):
        # TODO: Implement wifi connection strength
        self.wifi_connection = ""

    def update_controller_battery(self):
        # TODO: Implement controller battery strength
        self.controller_battery = ""

    def update(self):
        """
        Wrapper for updating the header icons
        """
        self.update_drone_gps()
        self.update_drone_battery()
        self.update_drone_connection()
        self.update_controller_gps()
        self.update_wifi()
        self.update_controller_battery()


class HomeButton(ActionPrevious):
    title = ''
    app_icon = os.path.abspath("icons/ArrowPrev.png")
    with_previous = False
    previous_image = os.path.abspath("icons/ArrowPrev.png")
    pack_width = 0


class Footer(ActionBar):
    next_icon = os.path.abspath("icons/ArrowNext.png")
    prev_icon = os.path.abspath("icons/ArrowPrev.png")
