import os

import bcolors
from kivy.properties import NumericProperty, StringProperty
from kivy.uix.actionbar import ActionButton
from kivy.uix.button import Button
from kivy.uix.carousel import Carousel
from kivy.uix.label import Label
from kivy.uix.popup import Popup
from kivy.uix.screenmanager import Screen
from kivy.uix.slider import Slider
from kivy.uix.widget import Widget
from requests import ConnectionError

from Software.GUI_V3.Components import *
from Software.libs.garden import MapView, MapMarker, MapMarkerPopup, MarkerMapLayer
from Software.util import settings, get_controller_geo_ip

coord_enabled = False
maneuver_icon = os.path.abspath("icons/MissingTexture.png")
maneuver_name = ""

try:
    """Put anything that requires an internet connection for the map here
    """
    controller_coords = get_controller_geo_ip()
    start_lat = NumericProperty(controller_coords[0])
    start_lon = NumericProperty(controller_coords[1])
except ConnectionError:
    """Put any connection failed code here
    """
    print bcolors.FAIL + "Failed to connect!" + bcolors.ENDC
    start_lat = NumericProperty(0)
    start_lon = NumericProperty(1)

slider_d = Slider()
slider_hl = Slider(orientation="vertical")
slider_hh = Slider(orientation="vertical")
label_d = Label(text="Diameter:" + str(int(slider_d.value)) + "m")  # TODO: Substitute with user defined unit
label_hl = Label(text="Low:" + str(int(slider_hl.value)) + "m")  # TODO: Substitute with user defined unit
label_hh = Label(text="High:" + str(int(slider_hh.value)) + "m")  # TODO: Substitute with user defined unit
button_exec = Button(text="Execute")
button_next = Button(text="Next")


def on_slide(instance, value):
    if instance is slider_d:
        label_d.text = "Diameter:" + str(int(value)) + "m"  # TODO: Substitute with user defined unit
    elif instance is slider_hl:
        label_hl.text = "Low:" + str(int(value)) + "m" # TODO: Substitute with user defined unit
    elif instance is slider_hh:
        label_hh.text = "High:" + str(int(value)) + "m"  # TODO: Substitute with user defined unit
        if slider_hl.value > instance.value:
            instance.value = slider_hl.value + 1


class ManeuverMarker(MapMarkerPopup):
    source = maneuver_icon
    anchor_y = 0.5


class DroneMarker(MapMarker):
    source = StringProperty(os.path.abspath("icons/DroneIcon.png"))
    lat, lon = start_lat, start_lon  # TODO: Update with a get_drone_gps() function
    anchor_y = 0.5


class ControllerMarker(MapMarker):
    source = StringProperty(os.path.abspath("icons/ControllerIcon.png"))
    lat, lon = start_lat, start_lon  # TODO: Update with a get_controller_gps() function
    anchor_y = 0.5


class Map(MapView):
    lat = start_lat
    lon = start_lon
    pause_on_action = False

    def __int__(self):
        self.zoom = 15


class MapScreen(Screen):
    content = Widget()
    diameter_slide = GridLayout(rows=3)
    height_slide_top = GridLayout(rows=2, cols=2)
    height_slide = GridLayout(rows=2)

    def __init__(self, **kwargs):
        super(MapScreen, self).__init__(**kwargs)
        self.map = Map()
        self.header = Header()
        self.footer = Footer()
        self.drone = DroneMarker()
        self.controller = ControllerMarker()
        self.maneuver = ManeuverMarker()

        self.add_widget(self.map)
        self.add_widget(self.header)
        self.add_widget(self.footer)

        self.drone.width = 40
        self.drone.height = 40
        self.map.add_marker(self.drone)

        self.controller.width = 40
        self.controller.height = 40
        self.map.add_marker(self.controller)

        self.maneuver.width = 40
        self.maneuver.height = 40
        self.map.add_marker(self.maneuver)

    def on_touch_down(self, touch):
        global coord_enabled

        if coord_enabled:
            self.maneuver.lat, self.maneuver.lon = self.map.get_latlon_at(touch.x, touch.y, self.map.zoom)
            self.maneuver.source = maneuver_icon
            self.map.trigger_update(False)
            self.create_popup().open()
            coord_enabled = False
        else:
            super(MapScreen, self).on_touch_down(touch)

    def create_popup(self):
        self.content.clear_widgets()
        self.diameter_slide.clear_widgets()
        self.height_slide_top.clear_widgets()
        self.height_slide.clear_widgets()

        if maneuver_name == "Orbit":
            self.content = GridLayout(rows=3)
            self.content.add_widget(label_d)
            self.content.add_widget(slider_d)
            button_exec.size_hint_y = 1
            self.content.add_widget(button_exec)
        elif maneuver_name == "Spiral":
            self.content = Carousel(direction="right")

            self.diameter_slide.add_widget(label_d)
            self.diameter_slide.add_widget(slider_d)
            self.diameter_slide.add_widget(button_next)

            label_hl.size_hint_y = .25
            self.height_slide_top.add_widget(label_hl)
            label_hh.size_hint_y = .25
            self.height_slide_top.add_widget(label_hh)
            self.height_slide_top.add_widget(slider_hl)
            self.height_slide_top.add_widget(slider_hh)

            self.height_slide.add_widget(self.height_slide_top)
            button_exec.size_hint_y = .25
            self.height_slide.add_widget(button_exec)

            self.content.add_widget(self.diameter_slide)
            self.content.add_widget(self.height_slide)

            button_next.bind(on_press=self.content.load_next)

        popup = Popup(title="%s Maneuver" % maneuver_name, title_align="center", content=self.content,
                      size_hint=(None, None), size=(200, 300), auto_dismiss=False)
        button_exec.bind(on_press=popup.dismiss)
        slider_d.bind(value=on_slide)
        slider_hl.bind(value=on_slide)
        slider_hh.bind(value=on_slide)

        return popup


class ManeuverButton(ActionButton):
    @staticmethod
    def update_marker_data(m):
        global coord_enabled, maneuver_name

        if m == "Orbit" or m == "Spiral":
            coord_enabled = True
            # print coord_enabled  # Debug
        elif m == "Follow Me":
            # TODO: Implement follow_me()
            pass
        elif m == "Follow This":
            # TODO: Implement follow_this()
            pass

        maneuver_name = m
