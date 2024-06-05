
char appByte;

    uint8_t dia;
    uint8_t mes;
    uint8_t ano;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;

void setup() {
  // put your setup code here, to run once:

  //baud rate
  Serial.begin(115200, SERIAL_8E1); 

  appByte = 'l';
  dia = 4;
  mes = 6;
  ano = 2024;
  hora = 17;
  minuto = 5;
  segundo = 30;
}


void SerialSendChar(char Byte){
  Serial.write(Byte);
}

void SerialSendTime(uint8_t dia, uint8_t mes, uint8_t ano, uint8_t hora, uint8_t minuto, uint8_t segundo){
  uint8_t auxarr[6] = {dia, mes, ano, hora, minuto, segundo};

  for(int i = 0; i < 6; i++){
    Serial.write(auxarr[i]) ;
  }

}

void SerialReceiveTime(uint8_t time_parameters[]){
    if(Serial.available() >= 6){

      //Recebendo os parâmetros de tempo e armazenando-os num vetor
      for(int i = 0; i < 6; i++){

        time_parameters[i] = Serial.read();

      }

    } else{
      Serial.println("Error in SerialReceiveTime");
    }
  }

char SerialReceiveStatus(){
  char current;
  if(Serial.available() > 0){
    unsigned int status = Serial.read();

    switch(status){
      //está irrigando
      case 0:
      current = 'V';
      break;
      //não está irrigando
      case 1:
      current = 'F';
      break;
      //erro desconhecido
      default:
      current = 'L';
      break;
    }

  } else{
    //erro desconhecido
    current = 'L';
  }

  return current;
}

void loop() {
  // put your main code here, to run repeatedly:
  

  switch(appByte){
    //ligar:
    case 'l':
    
    SerialSendChar('l');

    break;

    //desligar:
    case 'd':

    SerialSendChar('d');

    break;

    //marcar irrigação
    case 'm':
    SerialSendChar('m');
    //pedir dia, mês, ano, hora, minuto e segundo para o app

    //Enviar esses parâmetros para a ST
    SerialSendTime(dia, mes, ano, hora, minuto, segundo);

    break;

    //pedir o status da irrigação
    case 's':

    SerialSendChar('s');

    //Lê o status e classifica se é verdadeiro (irrigando) ou falso (desligado)
    char status;
    status = SerialReceiveStatus();

    if(status != 'L'){

      Serial.println(status);

    } 
    
    else{

      Serial.println("Error in SerialReceiveStatus");

    }
    
    break;

    //pedir o tempo da irrigação:
    case 'h':

    uint8_t time_parameters[6];
    SerialSendChar('h');
    SerialReceiveTime(time_parameters);
    for(int i = 0; i < 6; i++){
      Serial.println(time_parameters[i]);
    }

    //passa time_parameters para o app


    break;
    
  }



  }


