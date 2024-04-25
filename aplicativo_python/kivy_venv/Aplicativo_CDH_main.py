from datetime import datetime
from kivy.clock import Clock
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.app import App
from kivy.config import Config
from Wifi_Communication import Wifi_Communication as WC

sm = ScreenManager()

# Time update variables:

Data_Update = ""
Time_Update = ""


class CdhDesign(Screen):
    def TransitionC(self):
        sm.transition.direction = "left"
        sm.current = "Control"
    def TransitionS(self):
        sm.transition.direction = "left"
        sm.current = "Status"

class CdhDesignControl(Screen):
    def Return(self):
        sm.transition.direction = "right"
        sm.current = "Home"

    def CheckState(self):

        if WC.check_status():
#On_Button:
            self.ids.timer_1_on.disabled = True
# desativado

#Input_text:
            self.ids.timer_1.disabled = True #desativado
            self.ids.timer_1.text = str("Irrigação em andamento")
#OFF_button:
            self.ids.timer_1_off.disabled = False
        else:
#On_Button
            self.ids.timer_1_on.disabled = False
#Input text:
            self.ids.timer_1.disabled = False
            self.ids.timer_1.text = str("")
#OFF_button
            self.ids.timer_1_off.disabled = True

#Scheduling:

        if WC.check_status_schedule():
            self.ids.date__1.disabled = True
            self.ids.time1.disabled = True
            self.ids.schedule_on.disabled = True
            self.ids.schedule_off.disabled = False
            self.ids.date__1.text = str("data")
            self.ids.time1.text = str("hora")
        else:
            self.ids.date__1.disabled = False
            self.ids.time1.disabled = False
            self.ids.schedule_on.disabled = False
            self.ids.schedule_off.disabled = True
            self.ids.date__1.text = ""
            self.ids.time1.text = ""

# Irrigation Cronometer Controls --------> OK

    def Irrigation_cronometer(self):
        def updating_button_label_state(dt):
            self.ids.timer_1_label.text = "Ligar irrigação"
            self.ids.timer_1.hint_text = "Tempo de execução (M)"

        def CheckState2(dt):
            self.CheckState()

        if self.ids.timer_1.text != "" and self.ids.timer_1.text != "Horário inválido" and 0 < int(
                self.ids.timer_1.text) < 1000:

            stopwatch = str(self.ids.timer_1.text)

            WC.switchIrrigation("L", stopwatch)

            Clock.schedule_once(CheckState2, 2.0)

            Clock.schedule_once(updating_button_label_state, 1.5)
        else:
            def updating_text_input(dt):
                self.ids.timer_1.disabled = False
                self.ids.timer_1.hint_text = "Tempo de Execução (M)"
                self.ids.timer_1.text = ""

            self.ids.timer_1.disabled = True
            self.ids.timer_1.text = "Horário inválido"
            Clock.schedule_once(updating_text_input, 1.5)

    def Turning_off_irrigation(self):

        def CheckState2(dt):
            self.CheckState()

        WC.switchIrrigation("D", -1)
        Clock.schedule_once(CheckState2, 2.0)


# Schedule Controls

    def scheduling_irrigation(self):
        def updating_button_label_state(dt):
            self.ids.schedule_on.background_disabled_normal = "images/botao_vermelho_disabled.png"
            self.ids.schedule_on_label.text = "Agendar Irrigação"

        def CheckState2(dt):
            self.CheckState()


        def detect_index_date():
            i = 0
            for exp in self.ids.date__1.text.split("/"):
                i+=1
            return i

        def detect_index_time():
            i = 0
            for exp in self.ids.time1.text.split(":"):
                i+=1
            return i

        def validtime():
            temp1 = self.ids.time1.text.split(":")
            temp2 = self.ids.date__1.text.split("/")
            if 0 <= int(temp1[0]) < 24 and 0 < int(temp1[1]) < 60 and 1 <= int(temp2[0]) <= 30 and 1 <= int(temp2[1]) <= 12:
                return True
            else:
                return False



        if self.ids.date__1.text != "" and self.ids.time1.text != "" and detect_index_date() == 3 and validtime() == True and detect_index_time() == 2:
            temp1 = self.ids.date__1.text.split("/")
            temp3 = temp1[2] + "-" + temp1[1] + "-" + temp1[0] + " " + str(self.ids.time1.text)

            if temp3 > str(datetime.now()):
                date_irrigation_schedule = str(self.ids.date__1.text)
                time_irrigation_schedule = str(self.ids.time1.text)
                WC.switchIrrigationSchedule("L", date_irrigation_schedule, time_irrigation_schedule)
                self.ids.schedule_on_label.text = "Programado"
                self.ids.schedule_on.background_disabled_normal = "images/Botão verde.png"
                Clock.schedule_once(CheckState2, 2.0)
                Clock.schedule_once(updating_button_label_state, 1.5)
            else:
                self.ids.schedule_on_label.text = "Data inválida"
                self.ids.date__1.text = ""
                self.ids.time1.text = ""
                Clock.schedule_once(updating_button_label_state,1.5)
        else:
            self.ids.schedule_on_label.text = "Data inválida"
            self.ids.date__1.text = ""
            self.ids.time1.text = ""
            Clock.schedule_once(updating_button_label_state, 1.5)



    def Turning_off_irrigation_schedule(self):
        def Checkstate2(dt):
            self.CheckState()

        WC.switchIrrigationSchedule("D", "0", "0")
        Clock.schedule_once(Checkstate2, 2.0)

# Update time:

    def UpdatingMachineTime(self):
        def updating_button_label_state(dt):
            self.ids.label_clock.text = "Atualizar Horário"
            self.ids.update_button.disabled = False

        if self.ids.date2.text != "" and self.ids.time2.text != "":
            date_update = str(self.ids.date2.text)
            time_update = str(self.ids.time2.text)
            WC.switchIrrigationTime(date_update, time_update)

            self.ids.update_button.disabled = True

            self.ids.label_clock.text = "Horário Atualizado"
            self.ids.date2.text = ""
            self.ids.time2.text = ""
            Clock.schedule_once(updating_button_label_state, 3.0)
        else:
            self.ids.label_clock.text = "Horário inválido"
            Clock.schedule_once(updating_button_label_state, 1.5)

class CdhDesignStatus(Screen):
    def Return(self):
        sm.transition.direction = "right"
        sm.current = "Home"


    def LoadingServerStats(self):

#ESP Connection Status
        if WC.check_connection():
            self.ids.Label_Connection_Status.text = "Conectado"
            self.ids.connection_image.source = "images/Botao_verde_conexao.png"
            print("1")
        else:
            self.ids.Label_Connection_Status.text = "Desconectado"
            self.ids.connection_image.source = "images/Botao_vermelho_conexao.png"

#Irrigation Status (ON/OFF)

        if WC.check_status():
            self.ids.Irrigation_Status.text = "Ligado"
            print("2")
        else:
            self.ids.Irrigation_Status.text = "Desligado"

#Irrigation Running Time:

        time = WC.check_running_time()
        if time != -1:
            self.ids.Running_time.text = str(time)
            print("3")
        else:
            self.ids.Running_time.text = "-------------------"

#Time For Next Irrigation:

        if WC.check_remaining_time() != "-1":
            self.ids.Next_Irrigation.text = str(WC.check_remaining_time())
            print("4")
        else:
            self.ids.Next_Irrigation.text = "Sem Irrigação Programada"

    def Loop(self):
        self.LoadingServerStats()
        Clock.schedule_interval(lambda dt: self.LoadingServerStats(), 10)



class MainApp(App):
    def build(self):
        Config.set('graphics', 'width', "400")
        Config.set('graphics', 'height', "880")
        Config.write()
        self.load_kv("Aplicativo_CDH_main.kv")
        self.load_kv("Aplicativo_CDH_pagina_controle.kv")
        self.load_kv("Aplicativo_CDH_pagina_status.kv")

        sm.add_widget(CdhDesign(name="Home"))
        sm.add_widget(CdhDesignControl(name="Control"))
        sm.add_widget(CdhDesignStatus(name="Status"))
        return sm


if __name__ == '__main__':
    MainApp().run()
