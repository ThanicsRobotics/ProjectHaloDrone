from kivy.app import App
from kivy.core.window import Window
from kivy.lang import Builder
import os

from kivy.properties import StringProperty
from kivy.uix.actionbar import ActionPrevious
from kivy.uix.gridlayout import GridLayout
from kivy.uix.screenmanager import ScreenManager

from util import Colors, settings

from Screens.Map import MapScreen, ManeuverButton

colors = Colors()

Window.size = (480, 320)
Window.clearcolor = colors.white
Builder.load_file("Gui.kv")

cur_screen = 0

coord_enabled = False
flight_stats = {"vx": 0, "vy": 0, "vz": 0, "ax": 0, "ay": 0, "az": 0, "pitch": 0, "roll": 0, "yaw": 0, "alt": 0,
                "dist": 0, "vu": settings["general"]["units"]["velocity"],
                "au": settings["general"]["units"]["acceleration"], "mu": settings["general"]["units"]["measurement"],
                "du": u'\N{Degree Sign}'}


class HomeButton(ActionPrevious):
    title = 'Halo Controller'
    app_icon = os.path.abspath("icons/DroneIcon.png")
    with_previous = False


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


ms = MapScreen()
sm = ScreenManager()
sm.switch_to(ms)


class GUIApp(App):
    icon = os.path.abspath("icons/DroneIcon.png")
    title = "Halo Controller"

    def build(self):
        return sm


if __name__ == '__main__':
    GUIApp().run()
