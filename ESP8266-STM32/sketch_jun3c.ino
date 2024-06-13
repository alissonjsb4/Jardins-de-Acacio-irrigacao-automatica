uint8_t appByte;
uint8_t time_parameters[6];

uint8_t dia;
uint8_t mes;
uint8_t ano;
uint8_t hora;
uint8_t minuto;
uint8_t segundo;

const int BUTTON = 16; // GPIO16 (D0)

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP); // Use internal pull-up resistor for button pin

  appByte = 'h';
  dia = 4;
  mes = 6;
  ano = 24;
  hora = 17;
  minuto = 5;
  segundo = 30;
}

void SerialSendChar(uint8_t Byte) {
  Serial.write(Byte);
}

void SerialSendTime(uint8_t dia, uint8_t mes, uint8_t ano, uint8_t hora, uint8_t minuto, uint8_t segundo) {
  uint8_t auxarr[6] = {dia, mes, ano, hora, minuto, segundo};
  for (int i = 0; i < 6; i++) {
    Serial.write(auxarr[i]);
  }
}

void SerialReceiveTime(uint8_t time_parameters[]) {
  if (Serial.available() >= 6) {
    for (int i = 0; i < 6; i++) {
      time_parameters[i] = Serial.read();
    }
  } else {
    Serial.println("Error in SerialReceiveTime");
  }
}

uint8_t SerialReceiveStatus() {
  uint8_t status = 0xFF; // Unknown status
  if (Serial.available() > 0) {
    status = Serial.read();
    char current;
    switch (status) {
      case 0: current = 'V'; break;
      case 1: current = 'F'; break;
      default: current = 'T'; break;
    }
    Serial.println(current);
  } else {
    Serial.println('L'); // Unknown error
  }
  return status;
}

void Executer() {
  switch (appByte) {
    case 'l':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      break;
    case 'd':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      break;
    case 'm':
      SerialSendChar(appByte);
      SerialSendTime(dia, mes, ano, hora, minuto, segundo);
      break;
    case 's':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      delay(1000); // Adjust delay as needed
      SerialReceiveStatus();
      break;
    case 'h':
      SerialSendChar(appByte);
      SerialSendTime(0, 0, 0, 0, 0, 0);
      delay(1000); // Adjust delay as needed
      SerialReceiveTime(time_parameters);
      for (int i = 0; i < 6; i++) {
        Serial.println(time_parameters[i]);
      }
      break;
  }
}

void loop() {
  // Read the state of the button
  int reading = digitalRead(BUTTON);

  if(reading == HIGH){
    Executer();
  }

  //reading = HIGH;
}
