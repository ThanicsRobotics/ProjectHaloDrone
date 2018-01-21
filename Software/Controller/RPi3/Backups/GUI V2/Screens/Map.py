import os

from kivy.properties import NumericProperty, StringProperty
from kivy.uix.actionbar import ActionButton
from kivy.uix.screenmanager import Screen
from requests import ConnectionError

from libs.garden import MapView, MapMarker
from util import settings, get_controller_geo_ip

coord_enabled = False

maneuver_icon = StringProperty("")

try:
    """Put anything that requires an internet connection for the map here
    """
    controller_coords = get_controller_geo_ip()
    start_lat = controller_coords[0]
    start_lon = controller_coords[1]
except ConnectionError:
    """Put any connection failed code here
    """
    print "Failed to connect!"


class MapScreen(Screen):
    def __init__(self, **kw):
        super(MapScreen, self).__init__(**kw)


class ManeuverIcon(MapMarker):
    w = NumericProperty(40)
    h = NumericProperty(40)


class Map(MapView):
    lat = NumericProperty(start_lat)
    lon = NumericProperty(start_lon)
    marker_lat = NumericProperty(start_lat)
    marker_lon = NumericProperty(start_lon)
    marker_source = maneuver_icon
    maneuver_opacity = NumericProperty(0)

    def on_touch_down(self, touch):
        global coord_enabled

        if coord_enabled:
            self.update_marker_loc(touch)
            print "Placing marker at:", self.get_latlon_at(touch.x, touch.y, self.zoom)  # Debug
            coord_enabled = False
            self.maneuver_opacity = 1
            self._pause = False
        else:
            super(MapView, self).on_touch_down(touch)
            # self.maneuver_opacity = 0

    def update_marker_loc(self, touch):
        lat, lon = self.get_latlon_at(touch.x, touch.y, self.zoom)
        self.marker_lat = lat
        self.marker_lon = lon
        self.marker_source = maneuver_icon
        # print marker.source  # Debug


class DroneIcon(MapMarker):
    source = os.path.abspath("icons/DroneIcon.png")
    lat = start_lat
    lon = start_lon
    w = NumericProperty(40)
    h = NumericProperty(40)


class ControllerIcon(MapMarker):
    source = os.path.abspath("icons/DroneIcon.png")
    lat = start_lat
    lon = start_lon
    w = NumericProperty(40)
    h = NumericProperty(40)


class ManeuverButton(ActionButton):
    def update_marker_data(self, m):
        global maneuver_icon, coord_enabled

        if m == "Orbit":
            maneuver_icon = os.path.abspath("icons/Logo.png")
            coord_enabled = True
            # print "Setting ORBIT maneuver"  # Debug
            # print "Coordinates Enabled:", coord_enabled  # Debug
        # TODO: Implement rest of logic for other marker types
