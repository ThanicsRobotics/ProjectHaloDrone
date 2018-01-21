from kivy.app import App
from kivy.base import runTouchApp
from kivy.core.window import Window
from kivy.lang import Builder
import os
from kivy.uix.screenmanager import ScreenManager, Screen

from libs.garden import MapMarker
from libs.garden.mapview import MapView

Window.size = (480, 320)


class MapScreen(Screen):
    pass


class Map(MapView):
    pass


class ManeuverIcon(MapMarker):
    pass


class DroneIcon(MapMarker):
    pass


class ControllerIcon(MapMarker):
    pass


root = Builder.load_string("""
#:import os os

<MapScreen>:
    Map:
    ActionBar:

<Map>:
    lat: 40.597919
    lon: -105.09034099999997
    zoom: 13
    double_tap_zoom: True
            
<ActionBar>:
    pos_hint: {'bottom':1}
    ActionView:
        background_color: [7, 53, 226, 1]
        background_image: 
        use_separator: True
        ActionPrevious:
            title: 'Halo Controller'
            app_icon: os.path.abspath("icons/DroneIcon.png") 
            with_previous: False
        ActionButton:
            text: 'Btn0'
        ActionButton:
            text: 'Btn1'
        ActionButton:
            text: 'Btn2'
        ActionButton:
            text: 'Btn3'
        ActionButton:
            text: 'Btn4'
        ActionGroup:
            text: 'Maneuvers'
            ActionButton:
                text: 'Orbit'
            ActionButton:
                text: 'Spiral'
            ActionButton:
                text: 'Follow Me'
            ActionButton:
                text: 'Follow This'
""")

ms = MapScreen()
m = Map()
sm = ScreenManager()
sm.switch_to(ms)


class MapTestApp(App):
    def build(self):
        m.add_marker(DroneIcon(lat=40.597919, lon=-105.09034099999997, y_anchor=0.5))
        return sm


if __name__ == '__main__':
    MapTestApp().run()