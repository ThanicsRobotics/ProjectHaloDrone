from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import ListProperty
from kivy.config import Config
import random


Config.set('graphics', 'width', '480')
Config.set('graphics', 'height', '320')


class ScatterTextWidget(BoxLayout):
    text_color = ListProperty()

    def gen_rand_color(self, *args):
        color = [random.random() for i in xrange(3)] + [1]
        self.text_color = color


class TutorialApp(App):
    def build(self):
        return ScatterTextWidget()

if __name__ == "__main__":
    TutorialApp().run()
