
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
      printf("Error in HiTime\n");
    }
  }

char HiStatus(){
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

  //Pego os comandos do app:
  char appByte;
  appByte = 'l';
  

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
    dia = 4;
    unsigned int mes;
    mes = 6;
    unsigned int ano;
    ano = 2024;
    unsigned int hora;
    hora = 17;
    unsigned int minuto;
    minuto = 5;
    unsigned int segundo;
    segundo = 30;

    //Enviar esses parâmetros para a ST
    ByeTime(dia, mes, ano, hora, minuto, segundo);

    break;

    //pedir o status da irrigação
    case 's':

    ByeByte('s');

    //Lê o status e classifica se é verdadeiro (irrigando) ou falso (desligado)
    char status;
    status = HiStatus();

    if(status != 'L'){

      printf("%c", status);

    } 
    
    else{

      printf("Error in HiStatus");

    }
    
    break;

    //pedir o tempo da irrigação:
    case 'h':

    unsigned int time_parameters[6];
    ByeByte('h');
    HiTime(time_parameters);
    for(int i = 0; i < 6; i++){
      printf("%d", time_parameters[i]);
      printf("\n");
    }

    //passa time_parameters para o app


    break;
    
  }



  }


