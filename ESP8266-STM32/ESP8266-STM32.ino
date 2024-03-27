#include <HardwareSerial.h>
//#include <SoftwareSerial.h>

HardwareSerial SerialPort(0);

void setup() {
  // put your setup code here, to run once:
  SerialPort.begin(115200, SERIAL_8N1, SERIAL_FULL);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(SerialPort.available()){
    String Buffer = SerialPort.readString();
    Serial.println(Buffer);
  }
}
