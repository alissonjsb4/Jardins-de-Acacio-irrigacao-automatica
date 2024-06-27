uint8_t appByte;
uint8_t time_parameters[6];

uint8_t dia;
uint8_t mes;
uint8_t ano;
uint8_t hora;
uint8_t minuto;
uint8_t segundo;

const int V_led = 12;
const int F_led = 13;

const int BUTTON_L = 14; // GPIO14 (D5)
const int BUTTON_D = 5;  // GPIO5 (D1)
const int BUTTON_M = 4;  // GPIO4 (D2)
const int BUTTON_S = 0;  // GPIO0 (D3)
const int BUTTON_H = 2;  // GPIO2 (D4)

const unsigned long debounceDelay = 50; // the debounce time in milliseconds

unsigned long lastDebounceTime_L = 0; // the last time the output pin was toggled
unsigned long lastDebounceTime_D = 0;
unsigned long lastDebounceTime_M = 0;
unsigned long lastDebounceTime_S = 0;
unsigned long lastDebounceTime_H = 0;

int lastButtonState_L = HIGH; // the previous state of the button
int lastButtonState_D = HIGH;
int lastButtonState_M = HIGH;
int lastButtonState_S = HIGH;
int lastButtonState_H = HIGH;

void setup() {
  Serial.begin(115200);

  // Use internal pull-up resistors for button pins
  pinMode(BUTTON_L, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);
  pinMode(BUTTON_M, INPUT_PULLUP);
  pinMode(BUTTON_S, INPUT_PULLUP);
  pinMode(BUTTON_H, INPUT_PULLUP);

  pinMode(V_led, OUTPUT);
  pinMode(F_led, OUTPUT);

  appByte = 'h';
  dia = 4;
  mes = 6;
  ano = 24;
  hora = 17;
  minuto = 5;
  segundo = 30;
}

// Function to send a command character
void SerialSendChar(uint8_t Byte) {
  Serial.write(Byte);
}

// Function to send time parameters
void SerialSendTime(uint8_t dia, uint8_t mes, uint8_t ano, uint8_t hora, uint8_t minuto, uint8_t segundo) {
  uint8_t auxarr[6] = {dia, mes, ano, hora, minuto, segundo};
  for (int i = 0; i < 6; i++) {
    Serial.write(auxarr[i]);
  }
}

// Function to receive time parameters
void SerialReceiveTime(uint8_t time_parameters[]) {
  if (Serial.available() >= 6) {
    for (int i = 0; i < 6; i++) {
      time_parameters[i] = Serial.read();
    }
  } else {
    // Send an error message to the app
  }
}

// Function to receive irrigation status
uint8_t SerialReceiveStatus() {
  uint8_t status = 0xFF; // Unknown status
  char current;
  if (Serial.available() > 0) {
    status = Serial.read();
    switch (status) {
      // The chosen status pattern is 0 -> Irrigating and 1 -> Not irrigating
      case 0: current = 'V'; break;
      case 1: current = 'F'; break;
      default: current = 'T'; break;
    }
    // Send the 'current' variable to the app
  } else {
    // Send an error message to the app
  }
  return current;
}

void ShowStatus(char current){
  switch(current){
    case 'V':
    //Liga led verde
    digitalWrite(V_led, HIGH);
    delay(5000);
    digitalWrite(V_led, LOW);
    break;
    case 'F':
    //Liga led vermelho
    digitalWrite(F_led, HIGH);
    delay(5000);
    digitalWrite(F_led, LOW);
    break;
    case 'T':
    //Pisca led vermelho;
    for(int i = 0; i < 4; i++){

      digitalWrite(F_led, HIGH);
      delay(500);
      digitalWrite(F_led, LOW);
      
    }
    

  }
}
         
// Function with switch-case to modularize the code.
void Executer() {
  char current;
  switch (appByte) {
    // turn on:
    case 'l':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      break;
    // turn off
    case 'd':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      break;
    // schedule irrigation
    case 'm':
      SerialSendChar(appByte);
      SerialSendTime(dia, mes, ano, hora, minuto, segundo);
      break;
    // Receive irrigation status
    case 's':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      delay(1000); 
      current = SerialReceiveStatus();
      ShowStatus(current);
      break;
    // Receive back the time parameters
    case 'h':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      delay(1000); 
      SerialReceiveTime(time_parameters);
      // Send the parameters to the app
      break;
  }
}

void loop() {
  // These functions simulate sending commands from the app via buttons
  int reading_L = digitalRead(BUTTON_L);  
  int reading_D = digitalRead(BUTTON_D);
  int reading_M = digitalRead(BUTTON_M);
  int reading_S = digitalRead(BUTTON_S);
  int reading_H = digitalRead(BUTTON_H);

  if (reading_L != lastButtonState_L) {
    if (reading_L == LOW && (millis() - lastDebounceTime_L) > debounceDelay) {
      appByte = 'l';
      Executer();
      lastDebounceTime_L = millis();
    }
    lastButtonState_L = reading_L;
  }
  if (reading_D != lastButtonState_D) {
    if (reading_D == LOW && (millis() - lastDebounceTime_D) > debounceDelay) {
      appByte = 'd';
      Executer();
      lastDebounceTime_D = millis();
    }
    lastButtonState_D = reading_D;
  }
  if (reading_M != lastButtonState_M) {
    if (reading_M == LOW && (millis() - lastDebounceTime_M) > debounceDelay) {
      appByte = 'm';
      Executer();
      lastDebounceTime_M = millis();
    }
    lastButtonState_M = reading_M;
  }
  if (reading_S != lastButtonState_S) {
    if (reading_S == LOW && (millis() - lastDebounceTime_S) > debounceDelay) {
      appByte = 's';
      Executer();
      lastDebounceTime_S = millis();
    }
    lastButtonState_S = reading_S;
  }
  if (reading_H != lastButtonState_H) {
    if (reading_H == LOW && (millis() - lastDebounceTime_H) > debounceDelay) {
      appByte = 'h';
      Executer();
      lastDebounceTime_H = millis();
    }
    lastButtonState_H = reading_H;
  }
}
