import http.client
from bs4 import BeautifulSoup
import json

client = http.client.HTTPConnection("192.168.1.77")

#Get Methods:
def check_connection():
    client.request("GET", "/")
    resp = client.getresponse()
    if resp.status != 200:
        client.close()
        return False

    else:
        client.close()
        return True


def check_status():
    client.request("GET", "Irrigation-Status")        #Checa se a conexão com o webServer foi bem sucedida --- sem função correspondente na ESP
    status = client.getresponse().read().decode("utf-8").find("L")
    if status >= 0:
        client.close()
        return True
    else:
        client.close()
        return False

def check_status_schedule():
    client.request("GET", "/Schedule")                  #checks if there is an irrigation scheduled.
    status = client.getresponse().read().decode("utf-8").find("L") #if so, True, if dont, False. thus, this changes the display for the user

    if status >= 0:  # L for "ligado"
        client.close()
        return True
    else:
        client.close()
        return False

def check_running_time():               #Time for next Irrigation
    client.request("GET", "/Running-Time")
    html = client.getresponse().read().decode("utf=8")
    print(html)
    #html_content = BeautifulSoup(html, "html.parser")
    client.close()
    return html
def check_remaining_time():

    client.request("GET", "/Next-Irrigation")
    html = client.getresponse().read()
    html_content = BeautifulSoup(html, "html.parser").get_text()
    client.close()
    return html_content



#Post Methods:

def switchIrrigation(action, time):
    payload = {"command": action, "time": time}
    json_data = json.dumps(payload)
    print(json_data)
    client.request("POST", "/IrrigationDefStats", body=json_data)
    retorno = client.getresponse().read().decode("utf-8")
    print(retorno)
    client.close()

def switchIrrigationSchedule(action, date, time):
    payload = {"command": action, "date": date, "time": time}
    json_data = json.dumps(payload,separators=(",", ":"))
    print(json_data)
    client.request("POST", "/IrrigationDefSchedule", body=json_data)
    retorno = client.getresponse().read().decode("utf-8")
    print(retorno)
    client.close()

def switchIrrigationTime(date,time):
    payload = {"date": date, "time": time}
    json_data = json.dumps(payload,separators=(",", ":"))
    print(json_data)
    client.request("POST", "/IrrigationDefSysClock", body=json_data)
    client.close()