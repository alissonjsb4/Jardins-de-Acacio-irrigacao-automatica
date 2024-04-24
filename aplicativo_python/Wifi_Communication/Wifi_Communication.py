import http.client
from bs4 import BeautifulSoup

<<<<<<< HEAD

client = http.client.HTTPConnection("www.google.com")
=======
esp8266IP = ''      #preencher
esp8266PORT = 8080  #preencher

client = http.client.HTTPConnection(esp8266IP, esp8266PORT)
>>>>>>> f48496b4228dba14901a5d998491fff796f216ac

def check_connection():
    client.request("GET", "/")
    resp = client.getresponse()
    print("dajisjdai")
    if resp.status != 200:
        client.close()
        return False

    else:
        client.close()
        return True


def check_status():
    client.request("GET", "/status")
    status = client.getresponse().status
    if status == 404: # R for running, 404 only for test with google
        client.close()
        return True
    else:
        client.close()
        return False

def check_running_time():
<<<<<<< HEAD
    if check_connection() and check_status():
        client.request("GET", "/irrigation")
        html = client.getresponse().read()
        html_content = BeautifulSoup(html, "html.parser").get_text()
        client.close()
        return html_content
    else:
        return -1
=======
    client.request("GET", "/irrigation")
    time = client.getresponse().read().decode("utf-8")
    client.close()
    return time
>>>>>>> f48496b4228dba14901a5d998491fff796f216ac
