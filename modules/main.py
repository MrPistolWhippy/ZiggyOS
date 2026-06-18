#!/usr/bin/env python3
import os
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from shor_simulator import simulate_shor_factoring

class ZiggyShorHUD(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(orientation='vertical', padding=10, spacing=10, **kwargs)
        
        # Cyberpunk Mobile HUD Panel Display Labels
        self.add_widget(Label(text="THEOSI ZIGGYOS MOBILE HUD v100.0", font_size=24, color=(1, 0, 0.77, 1)))
        self.add_widget(Label(text="Enter Semi-Prime Target Number N:", font_size=16))
        
        self.input_n = TextInput(text="15", multiline=False, input_filter='int', font_size=20)
        self.add_widget(self.input_n)
        
        self.btn_crack = Button(text="LAUNCH SHOR FACTOR SWEEP", background_color=(0, 1, 0.5, 1), font_size=18)
        self.btn_crack.bind(on_press=self.execute_mobile_crack)
        self.add_widget(self.btn_crack)
        
        self.output_log = Label(text="Awaiting mobile runtime vector entry...", font_size=14, halign="center")
        self.add_widget(self.output_log)

    def execute_mobile_crack(self, instance):
        target_n = int(self.input_n.text) if self.input_n.text else 15
        # Interlock and execute your core offline mathematical factoring logic directly on the device
        import sys
        from io import StringIO
        old_stdout = sys.stdout
        sys.stdout = mystdout = StringIO()
        
        try:
            simulate_shor_factoring(target_n)
            sys.stdout = old_stdout
            self.output_log.text = mystdout.getvalue()
        except Exception as e:
            sys.stdout = old_stdout
            self.output_log.text = f"[-] Mobile Matrix Execution Fault: {e}"

class ZiggyOSApp(App):
    def build(self):
        return ZiggyShorHUD()

if __name__ == '__main__':
    ZiggyOSApp().run()
