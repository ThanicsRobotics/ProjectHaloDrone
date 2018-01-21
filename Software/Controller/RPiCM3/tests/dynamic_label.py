from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.clock import Clock
from kivy.core.window import Window
import time


class RootWidget(FloatLayout):

    def update(self, *args):
        value = time.time()
        print value
        self.ids.value_label.text = str(value)


class MainApp(App):

    def build(self):
        Window.size = (800, 480)
        r = RootWidget()
        Clock.schedule_interval(r.update, 1)
        print 'build running'
        return r

    def on_pause(self):
        return True

if __name__ == '__main__':
    MainApp().run()