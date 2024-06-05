#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>



const char* ssid = "LESC";
const char* password = "A33669608F";
WiFiUDP udp;
NTPClient ntp(udp,"a.st1.ntp.br",-3 * 3600,60000);

//char* scheduled;



void timeLeft(){
  int hora = ntp.getHours();
  int minutos = ntp.getMinutes();
  int Seconds = ntp.getSeconds();

  // ainda vai ser implementado
}


ESP8266WebServer server(80);





void handleRootServer(){
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Irrigation System Control</title>
      <style>
          body {
              font-family: Arial, sans-serif;
              text-align: center;
              margin-top: 50px;
          }
          .container {
              max-width: 600px;
              margin: 0 auto;
              padding: 20px;
              border: 1px solid #ccc;
              border-radius: 10px;
              box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
          }
          h1, h2 {
              color: #333;
          }
          button {
              display: block;
              width: 100%;
              padding: 10px;
              margin: 10px 0;
              font-size: 16px;
              color: #fff;
              background-color: #007BFF;
              border: none;
              border-radius: 5px;
              cursor: pointer;
          }
          button:hover {
              background-color: #0056b3;
          }
          #result {
              margin-top: 20px;
              padding: 10px;
              border: 1px solid #ccc;
              border-radius: 5px;
              background-color: #f9f9f9;
          }
      </style>
      <script>
          async function checkConnection() {
              let response = await fetch('/');
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function checkStatus() {
              let response = await fetch('/status');
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function checkStatusSchedule() {
              let response = await fetch('/schedule');
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function checkRunningTime() {
              let response = await fetch('/irrigation');
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function checkRemainingTime() {
              let response = await fetch('/remaining-time');
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function switchIrrigation(action, time) {
              let response = await fetch('/switch-irrigation', {
                  method: 'POST',
                  headers: {
                      'Content-Type': 'application/x-www-form-urlencoded',
                  },
                  body: command=${action}&time=${time}
              });
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function switchIrrigationSchedule(action, date, time) {
              let response = await fetch('/switch-irrigation-schedule', {
                  method: 'POST',
                  headers: {
                      'Content-Type': 'application/x-www-form-urlencoded',
                  },
                  body: command=${action}&date=${date}&time=${time}
              });
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }

          async function switchIrrigationTime(date, time) {
              let response = await fetch('/switch-irrigation-time', {
                  method: 'POST',
                  headers: {
                      'Content-Type': 'application/x-www-form-urlencoded',
                  },
                  body: date=${date}&time=${time}
              });
              let text = await response.text();
              document.getElementById('result').innerText = text;
          }
      </script>
  </head>
  <body>
      <div class="container">
          <h1>Controle do Sistema de Irrigação</h1>
          <div class="section">
              <h2>Verificações</h2>
              <button onclick="checkConnection()">Verificar Conexão</button>
              <button onclick="checkStatus()">Verificar Status</button>
              <button onclick="checkStatusSchedule()">Verificar Status da Programação</button>
              <button onclick="checkRunningTime()">Verificar Tempo de Funcionamento</button>
              <button onclick="checkRemainingTime()">Verificar Tempo Restante</button>
              <div id="result"></div>
          </div>
          <div class="section">
              <h2>Comandos</h2>
              <button onclick="switchIrrigation('ligar', 30)">Ligar Irrigação por 30 min</button>
              <button onclick="switchIrrigationSchedule('agendar', '2024-12-01', '06:00')">Agendar Irrigação</button>
              <button onclick="switchIrrigationTime('2024-12-01', '06:00')">Definir Hora do Sistema</button>
          </div>
      </div>
  </body>
  </html>
  )rawliteral";
  server.send(200,"text/html",html); //Manda o codigo 200 "OK",um content-type: "text/html", e o codigo do servidor em html para o root "/"
}


void handleIrrigationStatus(){
  String Status = "L";// requisita a STM o status atual do jardim, se esta ligado("L") ou nao ("D")
  server.send(200,"text-plain", Status);
}

void handleRunningTime(){
  //aqui a ESP requisita por comunicação Serial o clock do STM para que este diga há quanto tempo está irrigando, podemos discutir quem faz o calculo do cloc (ESP ou STM)
  // vou denotar pela variavel "time" para ser o valor, esse valor tem que ser em string ou sao passados dados por JSON, sendo assim a STM pode so mandar dados inteiros.
  String tempo = "15 minutos";
  server.send(200,"text/plain",tempo);
}

String irrigacaoProgramada = "L"; //L para está programado, D para não está desligado

void handleScheduleStatus(){
  //aqui checa se tem uma irrigação programada e passa para o programa
  server.send(200,"text/plain",irrigacaoProgramada);
}
void handleRemainingTime(){
  //ainda vai ser implementado completamente
  server.send(200,"text/plain","teste");
}
void handleDefIrrigation(){
  String json = server.arg("plain"); //Tratamento de arquivo Json
  StaticJsonDocument<16> doc;
  deserializeJson(doc,json);
  String action = doc["command"];
  String time = doc["time"];
  // Essa informação deve ser mandada para o STM
  server.send(200,"text/plain", "command: " + action + " in " + time + " minutes");
  Serial.println(action);
  Serial.println(time);
}
void handleDefSchedule(){
  String json = server.arg("plain");
  StaticJsonDocument<32> doc;
  deserializeJson(doc,json);
  
  String action = doc["command"];
  String time = doc["time"];
  String date = doc["date"];
  // essa informação deve ser tratada de forma apropriada e enviada para a STM por comunicação Serial
  server.send(200,"text/plain", "command: " + action + " in " + time + " minutes " + "date: " + date);
  Serial.println(action);
  Serial.println(time);
  Serial.println(date);
}
void handleDefSysClock(){
  String json = server.arg("plain");
  StaticJsonDocument<16> doc;
  deserializeJson(doc,json);
  String date = doc["date"];    // recebe um "payload" que contem o campo command e uma key assosciad --> date
  String time = doc["time"];        // recebe um "payload" que contem o campo command e uma key assosciad --> time
  // essa informação deve ser enviada para a STM por comunicação serial.
  server.send(200,"text/plain", "date: " + date + " time: " + time);
  Serial.println(date);
  Serial.println(time);
}
void handleNotFound(){
  server.send(404,"text/plain", "404 page not found");  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to:");
    Serial.print(ssid);
    Serial.println("....");
    delay(1000);
  }
  
  Serial.println("Connected to:");
  Serial.print(ssid);
  Serial.println("Server ip:");
  Serial.print(WiFi.localIP());
  ntp.begin();
  ntp.forceUpdate();
  Serial.println("NTP atualizado");
  server.on("/",HTTP_GET, handleRootServer);
  server.on("Irrigation-Status",HTTP_GET,handleIrrigationStatus);
  server.on("/Schedule",HTTP_GET, handleScheduleStatus);
  server.on("/Running-Time",HTTP_GET, handleRunningTime);
  server.on("/Next-Irrigation",HTTP_GET, handleRemainingTime);
  server.on("/IrrigationDefStats",HTTP_POST, handleDefIrrigation);
  server.on("/IrrigationDefSchedule",HTTP_POST, handleDefSchedule);
  server.on("/IrrigationDefSysClock",HTTP_POST, handleDefSysClock);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Server Started");
}

void loop() {
  server.handleClient();

}
