from datetime import datetime
from kivy.clock import Clock
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.app import App
from kivy.config import Config
from Wifi_Communication import Wifi_Communication as WC

sm = ScreenManager()

# irrigation variables

Cronometer_timer = -1
# em minutos

Irrigation_state = True
# True --> irrigando

# False --> Não irrigando

# schedule variables
date_irrigation_schedule = ""
time_irrigation_schedule = ""

Schedule_Irrigation_State = True
# True --> tem uma irração marcada
# False --> Não há uma irrigação marcada

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

        if Irrigation_state == True:
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

        if Schedule_Irrigation_State == True:
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

# Irrigation Cronometer Controls

    def Irrigation_cronometer(self):
        def updating_button_label_state(dt):
            self.ids.timer_1_label.text = "Ligar irrigação"
            self.ids.timer_1.hint_text = "Tempo de execução (M)"


        if self.ids.timer_1.text != "" and self.ids.timer_1.text != "Horário inválido" and int(self.ids.timer_1.text) > 0 and int(self.ids.timer_1.text) < 1000:
            global Cronometer_timer
            global Irrigation_state
            Cronometer_timer = int(self.ids.timer_1.text)
            self.ids.timer_1_off.disabled = False
            self.ids.timer_1.text = ""
            self.ids.timer_1_label.text = "Ligado"
            Irrigation_state = True
            self.CheckState()
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
        global Irrigation_state
        Irrigation_state = False
        self.CheckState()


# Schedule Controls

    def scheduling_irrigation(self):
        def updating_button_label_state(dt):
            self.ids.schedule_on_label.text = "Agendar Irrigação"


        global date_irrigation_schedule
        global time_irrigation_schedule

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
            if 0 <= int(temp1[0]) < 24 and 0 < int(temp1[1]) < 60:
                return True
            else:
                return False



        if self.ids.date__1.text != "" and self.ids.time1.text != "" and detect_index_date() == 3 and validtime() == True and detect_index_time() == 2:
            temp1 = self.ids.date__1.text.split("/")
            temp3 = temp1[2] + "-" + temp1[1] + "-" + temp1[0] + " " + str(self.ids.time1.text)

            if temp3 > str(datetime.now()):
                date_irrigation_schedule = str(self.ids.date__1.text)
                time_irrigation_schedule = str(self.ids.time1.text)
                self.ids.schedule_on_label.text = "Programado"
                self.ids.date__1.text = ""
                self.ids.time1.text = ""
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
        global Schedule_Irrigation_State
        Schedule_Irrigation_State = False
        self.CheckState()

# Update time:

    def UpdatingMachineTime(self):
        def updating_button_label_state(dt):
            self.ids.label_clock.text = "Atualizar Horário"

        global Data_Update
        global Time_Update
        if self.ids.date2.text != "" and self.ids.time2.text != "":
            Data_Update = str(self.ids.date2.text)
            Time_Update = str(self.ids.time2.text)
            print(Data_Update + "\n" + Time_Update)
            self.ids.label_clock.text = "Horário Atualizado"
            self.ids.date2.text = ""
            self.ids.time2.text = ""
            Clock.schedule_once(updating_button_label_state, 1.5)
        else:
            self.ids.label_clock.text = "Horário inválido"
            Clock.schedule_once(updating_button_label_state, 1.5)

class CdhDesignStatus(Screen):
    def Return(self):
        sm.transition.direction = "right"
        sm.current = "Home"


    def LoadingServerStats(self):
        if WC.check_connection():
            self.ids.Label_Connection_Status.text = "Conectado"
            self.ids.connection_image.source = "images/Botao_verde_conexao.png"
        else:
            self.ids.Label_Connection_Status.text = "Desconectado"
            self.ids.connection_image.source = "images/Botao_vermelho_conexao.png"

#Irrigation Status (ON/OFF)

        if WC.check_status():
            self.ids.Irrigation_Status.text = "Ligado"
        else:
            self.ids.Irrigation_Status.text = "Desligado"

#Irrigation running time:

        time = WC.check_running_time()
        if time != -1:
            self.ids.Running_time.text = str(time)


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
