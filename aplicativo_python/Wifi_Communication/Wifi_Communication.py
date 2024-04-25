import http.client
from bs4 import BeautifulSoup
import json

client = http.client.HTTPConnection("www.google.com")

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
    client.request("GET", "/status")
    status = client.getresponse().status
    if status == 404: # L for "ligado", 404 only for test with Google
        client.close()
        return True
    else:
        client.close()
        return False

def check_status_schedule():
    client.request("GET", "/schedule")
    status = client.getresponse().status
    if status == 404:  # L for "ligado", 404 only for testing
        client.close()
        return True
    else:
        client.close()
        return False

def check_running_time():
    client.request("GET", "/irrigation")
    html = client.getresponse().read()
    html_content = BeautifulSoup(html, "html.parser").get_text()
    client.close()
    return html_content
def check_remaining_time():

    client.request("GET", "/schedule")
    html = client.getresponse().read()
    html_content = BeautifulSoup(html, "html.parser").get_text()
    client.close()
    return html_content



#Post Methods:

def switchIrrigation(action, time):
    payload = {"command": action, "time": time}
    json_data = json.dumps(payload)
    print(json_data)
    client.request("POST", "/irrigation", body=json_data)
    client.close()

def switchIrrigationSchedule(action, date, time):
    payload = {"command": action, "date": date, "time": time}
    json_data = json.dumps(payload,separators=(",", ":"))
    print(json_data)
    client.request("POST", "/schedule", body=json_data)
    client.close()

def switchIrrigationTime(date,time):
    payload = {"date": date, "time": time}
    json_data = json.dumps(payload,separators=(",", ":"))
    print(json_data)
    client.request("POST", "/sysclock", body=json_data)
    client.close()