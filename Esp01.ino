#include <WiFi.h>
#include <WiFiClient.h>
#include <HardwareSerial.h>

const char* ssid = "LESC";
const char* password = "YourWiFiPassword"; //A33669608F
const int port = 8080;

WiFiServer server(port);
HardwareSerial SerialUART(1);

void connectWifi(const char* ssid, const char* password){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
  }
}

String getMessage(WiFiClient& client){
    while (client.connected()) {
        if (client.available()) {
            String message = client.readStringUntil('\n');
            return message;
        }
    }
    client.stop();
    return "";
}

void setup() {
    Serial.begin(115200);
    SerialUART.begin(9600, SERIAL_8N1, 16, 17); // UART pins for ESP32 (TX, RX)
    connectWifi(ssid, password);
    server.begin();
}

void loop() {
    //Check if ESP is connected to WiFi
    //If not, reconnect
    if(WiFi.status() != WL_CONNECTED){
      connectWifi(ssid, password);
      delay(5000);
    }

    //Check if the Server has a client connected
    //and bridge the request from Python -> ESP -> STM32
    WiFiClient client = server.available();
    if(client){
      //Get the request from client (Python app)
      //And send it to the microcontroller (STM32)
      String request = getMessage(client);
      SerialUART.write(request.c_str(), request.length());
    }

    //Future implementation of STM32 response and status of activity

}
