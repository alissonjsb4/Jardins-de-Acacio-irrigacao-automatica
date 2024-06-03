
void setup() {
  // put your setup code here, to run once:

  //baud rate
  Serial.begin(115200, SERIAL_8E1); 

}

void ByeByte(char ByteToGo){
  Serial.write(ByteToGo);
}

void ByeTime(unsigned int dia, unsigned int mes, unsigned int ano, unsigned int hora, unsigned int minuto, unsigned int segundo){
  unsigned int auxarr[6] = {dia, mes, ano, hora, minuto, segundo};

  for(int i = 0; i < 6; i++){
    Serial.write(auxarr[i]) ;
  }

}

void HiTime(unsigned int time_parameters[]){
    if(Serial.available() >= 6){

      //Recebendo os parâmetros de tempo e armazenando-os num vetor
      for(int i = 0; i < 6; i++){

        time_parameters[i] = Serial.read();

      }

    } else{
      // retorna uma mensagem de erro
    }
  }

char HiStatus(){
  char current;
  if(Serial.available > 0){
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

  //Pego os comandos do app:
  char appByte;

  switch(appByte){
    //ligar:
    case 'l':

    ByeByte('l');

    break;

    //desligar:
    case 'd':

    ByeByte('d');

    break;

    //marcar irrigação
    case 'm':
    ByeByte('m');
    //pedir dia, mês, ano, hora, minuto e segundo para o app
    unsigned int dia;
    unsigned int mes;
    unsigned int ano;
    unsigned int hora;
    unsigned int minuto;
    unsigned int segundo;

    //Enviar esses parâmetros para a ST
    ByeTime(dia, mes, ano, hora, minuto, segundo);

    break;

    //pedir o status da irrigação
    case 's':

    ByeByte('s');

    //Lê o status e classifica se é verdadeiro (irrigando) ou falso (desligado)
    char current = HiStatus();

    if(current != 'L'){

      ByeByte(current);

    } 
    
    else{

      // Retorna uma mensagem de erro pro app;

    }
    
    break;

    //pedir o tempo da irrigação:
    case 'h':

    unsigned int time_parameters[6];
    ByeByte('h');
    HiByte(time_parameters);

    //passa time_parameters para o app


    break;
    
  }



  }


