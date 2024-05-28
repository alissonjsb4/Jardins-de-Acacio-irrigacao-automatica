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
        sm.transition.direction = "left"        #Funções de transição para o botão de troca de aba
        sm.current = "Control"                  #TransitionC --> refere-se a ida para a pagina de controle
    def TransitionS(self):                      #TransitionS --> refere-se a ida para a pagina de status
        sm.transition.direction = "left"
        sm.current = "Status"

class CdhDesignControl(Screen):
    def Return(self):                           #Função de retorno ao menu principal
        sm.transition.direction = "right"
        sm.current = "Home"

    def CheckState(self):               #Função que seta os parâmetros corretos baseado nos status atual da irrigação, previndo bugs ou más intenções/escolhas do usuário
                                        #Tem como objetivo evitar a quebra do programa caso sejam passados muitos argumentos para as funções, evitando a quebra de paralelismo APP <-> ESP
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
        #Essa função tem como papel mandar a ESP um comando de ligar a Irrigação e passa como parametro um char: "L" e o tempo o qual deverá continuar ligado, definitido na variavel stopwatch



        def updating_button_label_state(dt):                        #Função auxiliar, ela é chamada para que haja um time out entre as funções que enviam informações a ESP
            self.ids.timer_1_label.text = "Ligar irrigação"         #sem comprometer a identidade visual do aplicativo
            self.ids.timer_1.hint_text = "Tempo de execução (M)"

        def CheckState2(dt):                                        #Função redundante: Revisar
            self.CheckState()

        if self.ids.timer_1.text != "" and self.ids.timer_1.text != "Horário inválido" and 0 < int(
                self.ids.timer_1.text) < 1000:                              #if de proteção para que os dados enviados não sejam errôneos, sendo o intervalo de minutos a serem enviados à esp 0 < dt < 1000

            stopwatch = str(self.ids.timer_1.text)

            WC.switchIrrigation("L", stopwatch)

            Clock.schedule_once(CheckState2, 2.0)

            Clock.schedule_once(updating_button_label_state, 1.5)
        else:
            def updating_text_input(dt):                                #Função visual: novamente uma função auxiliar para que haja um timeout entre as informações enviadas
                self.ids.timer_1.disabled = False                       #e para que os parâmetros voltem ao seu estado padrão
                self.ids.timer_1.hint_text = "Tempo de Execução (M)"
                self.ids.timer_1.text = ""

            self.ids.timer_1.disabled = True
            self.ids.timer_1.text = "Horário inválido"
            Clock.schedule_once(updating_text_input, 1.5)

    def Turning_off_irrigation(self):
        #Função que tem como papel desligar a irrigação. Manda para a ESP parametros: "D" e -1

        def CheckState2(dt):         # -----> Função redundante: Revisar
            self.CheckState()

        WC.switchIrrigation("D", -1)
        Clock.schedule_once(CheckState2, 2.0)


# Schedule Controls

    def scheduling_irrigation(self):
        def updating_button_label_state(dt):
            self.ids.schedule_on.background_disabled_normal = "images/botao_vermelho_disabled.png"      #Função auxiliar visual, seta de volta para os parâmetros originais.
            self.ids.schedule_on_label.text = "Agendar Irrigação"

        def CheckState2(dt):
            self.CheckState()


        def detect_index_date():                            #Funções auxiliares para formatação das informações recebidas pelo input do aplicativo
            i = 0
            for exp in self.ids.date__1.text.split("/"):    #Checa se o usuario mandou a data em formato dd/mm/aaaa
                i+=1
            return i

        def detect_index_time():
            i = 0
            for exp in self.ids.time1.text.split(":"):      #checa se o usuario mandou a data em formato hh:mm
                i+=1
            return i

        def validtime():                                    #checa se o usuario mandou uma hora e data correta. horas até 23:59 e dias até dia 30. futura implementação de sistema complexo de meses e anos bissextos
            temp1 = self.ids.time1.text.split(":")
            temp2 = self.ids.date__1.text.split("/")
            if 0 <= int(temp1[0]) < 24 and 0 < int(temp1[1]) < 60 and 1 <= int(temp2[0]) <= 30 and 1 <= int(temp2[1]) <= 12:
                return True
            else:
                return False



        if self.ids.date__1.text != "" and self.ids.time1.text != "" and detect_index_date() == 3 and validtime() == True and detect_index_time() == 2: #checa os requisitos para que nao gere conflitos, por exemplo:
            temp1 = self.ids.date__1.text.split("/")                                                                                                    #Data: dd/mm/aaaa, hora: hh:mm
            temp3 = temp1[2] + "-" + temp1[1] + "-" + temp1[0] + " " + str(self.ids.time1.text) #---> formatação para que seja possivel comparar as datas e o usuario nao poder enviar para o sistema de agendamento
                                                                                                #uma data invalida, por exemplo, no passado.
            if temp3 > str(datetime.now()):
                date_irrigation_schedule = str(self.ids.date__1.text)
                time_irrigation_schedule = str(self.ids.time1.text)
                WC.switchIrrigationSchedule("L", date_irrigation_schedule, time_irrigation_schedule) #é passado para a ESP os parametros "L" para ligado, str(data) e str(hora)
                self.ids.schedule_on_label.text = "Programado"                      #Mudanças visuais para o usuario
                self.ids.schedule_on.background_disabled_normal = "images/Botão verde.png"
                Clock.schedule_once(CheckState2, 2.0)               #retorna os padrões do aplicativo, baseado na função checkState
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

        WC.switchIrrigationSchedule("D", "0", "0")      #Desligando irrigação programada: é passado os parametros: "D" para desligado, str(0), str(0)
        Clock.schedule_once(Checkstate2, 2.0)

# Update time:

    def UpdatingMachineTime(self):                                          #Função para atualizar o clock da STM de forma indireta pela ESP
        def updating_button_label_state(dt):            #Função auxiliar para voltar aos padrões quando necessario.
            self.ids.label_clock.text = "Atualizar Horário"
            self.ids.update_button.disabled = False

        if self.ids.date2.text != "" and self.ids.time2.text != "":         #Garantir que nao sejam cometidos erros pelo usuario
            date_update = str(self.ids.date2.text)
            time_update = str(self.ids.time2.text)
            WC.switchIrrigationTime(date_update, time_update)               #é enviado à ESP os parâmetros: str(data),str(hora)

            self.ids.update_button.disabled = True

            self.ids.label_clock.text = "Horário Atualizado"                #Funções voltadas a experiencia do usuario, animações e atualização com base na interação do usuario com o sistema
            self.ids.date2.text = ""
            self.ids.time2.text = ""
            Clock.schedule_once(updating_button_label_state, 3.0)
        else:
            self.ids.label_clock.text = "Horário inválido"
            Clock.schedule_once(updating_button_label_state, 1.5)

class CdhDesignStatus(Screen):
    def Return(self):
        sm.transition.direction = "right" #Função de retornar para o menu principal
        sm.current = "Home"


    def LoadingServerStats(self):

#ESP Connection Status
        if WC.check_connection():                                                   #Checa a conexão com a ESP e mostra para o usuario o status da conexão
            self.ids.Label_Connection_Status.text = "Conectado"
            self.ids.connection_image.source = "images/Botao_verde_conexao.png"
            print("1")
        else:
            self.ids.Label_Connection_Status.text = "Desconectado"
            self.ids.connection_image.source = "images/Botao_vermelho_conexao.png"

#Irrigation Status (ON/OFF)

        if WC.check_status():                               #Checa com a ESP e mostra para o usuario o status da irrigação do jardim, se esta ligado ou nao
            self.ids.Irrigation_Status.text = "Ligado"
            print("2")
        else:
            self.ids.Irrigation_Status.text = "Desligado"

#Irrigation Running Time:

        time = WC.check_running_time()                      #Checa com a ESP o tempo que está irrigando e mostra para o usuário.
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
        Clock.schedule_interval(lambda dt: self.LoadingServerStats(), 10)   #função que deixa em loop a função que checa todos os Status, programada para acontecer a cada 10s. essa função precisa de melhoria

#   Acima há todas as telas do aplicativo, cada uma contendo suas respectivas funções
#   que operam somente em suas classes, ou seja, telas

class MainApp(App):
    def build(self):
        Config.set('graphics', 'width', "400")      #Setando configurações padrão para o aplicativo: Resolução
        Config.set('graphics', 'height', "880")
        Config.write()
        self.load_kv("Aplicativo_CDH_main.kv")              #Chamando os arquivos .kv para que sejam lidos e a formatação ocorra
        self.load_kv("Aplicativo_CDH_pagina_controle.kv")   #de forma esperada
        self.load_kv("Aplicativo_CDH_pagina_status.kv")

        sm.add_widget(CdhDesign(name="Home"))
        sm.add_widget(CdhDesignControl(name="Control"))
        sm.add_widget(CdhDesignStatus(name="Status"))
        return sm

#   Acima há a main class, que é a classe de chamada da biblioteca kivy
#   nela voce chama quais telas voce quer adicionar ao seu aplicativo.
#   A partir do metodo screenManager do kivy voce pode adicionar e nomear quais sao as telas que se deseja dispor no aplicativo.
#   Ao retornar SM(screenManager) voce está retornando um objeto que é responsável por armazenar as telas
#   e exibir-lás na ordem que melhor lhe acomodar

if __name__ == '__main__':
    MainApp().run()
