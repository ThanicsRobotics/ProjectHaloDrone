from kivy.app import App
from kivy.core.window import Window
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager

from Screens.Diagnostics import *
from Screens.FlightStats import *
from Screens.Map import *
from Screens.Settings import *
from Screens.Video import *
from Components import *
from Software.Controller.RPi3.util import Colors

colors = Colors()

Window.size = (480, 320)
Window.clearcolor = colors.white
Builder.load_file("GUI.kv")


sm = ScreenManager()
fs = FlightStatsScreen(name="flight_stats")
m = MapScreen(name="map")
v = VideoScreen(name="video")
d = DiagnosticsScreen(name="diagnostics")
s = SettingsScreen(name="settings")

sm.add_widget(fs)
sm.add_widget(m)
sm.add_widget(v)
sm.add_widget(d)
sm.add_widget(s)
sm.current = "map"


class GUIApp(App):
    icon = os.path.abspath("icons/DroneIcon.png")
    title = "Halo Controller"

    def build(self):
        return sm


if __name__ == "__main__":
    GUIApp().run()
