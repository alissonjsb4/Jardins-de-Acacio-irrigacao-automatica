#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";
IPAddress appServerIP(192, 168, 1, 100);  // IP address of the app server
const int appServerPort = 80;             // Port of the app server
const int uartBaudRate = 9600;            // Baud rate for UART communication
SoftwareSerial uart(2, 3);                // RX, TX pins for UART communication

void setup() {
  Serial.begin(115200);
  uart.begin(uartBaudRate);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Print ESP-01 IP address
  Serial.print("ESP-01 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Check for incoming messages from the app
    WiFiClient client;
    if (client.connect(appServerIP, appServerPort)) {
      if (client.available() > 0) {
        String message = client.readStringUntil('\n');
        Serial.print("Received message from app: ");
        Serial.println(message);
        // Forward the message to another device via UART
        uart.println(message);
      }
      client.stop();
    }
  } else {
    // Reconnect to WiFi if connection is lost
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}
