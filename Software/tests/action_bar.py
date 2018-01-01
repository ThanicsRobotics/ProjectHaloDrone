from kivy.base import runTouchApp
from kivy.core.window import Window
from kivy.lang import Builder

Window.size = (480, 320)

runTouchApp(Builder.load_string('''
#: import os os

ActionBar:
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
'''))