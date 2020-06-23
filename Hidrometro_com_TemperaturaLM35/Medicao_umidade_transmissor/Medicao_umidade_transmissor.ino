#include <Wire.h>
#include <RTClib.h>
#include <VirtualWire.h>


#define S_Temp    A0

#define S_Umidade1 A1
#define S_Umidade2 A2

#define RF_pin      11

#define Led_Verm    5
#define Led_Verd   6
#define Led_Azul   7

#define TEMPO 1

long int tempoCompensado = 0;

float medicoes[8];
float aux = 0;


uint8_t mensagem[10];
uint8_t lenMensagem = sizeof(mensagem);

uint8_t   *dia         = &mensagem[0];
uint8_t   *mes         = &mensagem[1];
uint8_t   *ano         = &mensagem[2];
uint8_t   *diaSemana   = &mensagem[3];
uint8_t   *hora        = &mensagem[4];
uint8_t   *minuto      = &mensagem[5];
uint8_t   *segundo     = &mensagem[6];
uint8_t   *temperatura = &mensagem[7];
uint8_t   *umidade1    = &mensagem[8];
uint8_t   *umidade2    = &mensagem[9];

//DECLARAÇÃO DOS DIAS DA SEMANA  - PESQUISAR A NECESSIDADE DESSA PARTE !!!!!
char daysOfTheWeek[7][12] = {  "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

RTC_DS3231 rtc;


void setup(){

  // INPUT DO SENSOR DE TEMPERATURA 
  pinMode(S_Temp, INPUT);

  // OUTPU DOS LEDS EM PINOS PWM
  pinMode(Led_Verm, OUTPUT);
  pinMode(Led_Verd, OUTPUT);
  pinMode(Led_Azul, OUTPUT);

  // INPUT DOS SENSORES DE UMIDADE
  pinMode(S_Umidade1, INPUT);
  pinMode(S_Umidade2, INPUT);
  
  Serial.begin(9600);

  if(!rtc.begin()) {
    //REINICIA O ARDUINO APOS 1 SEGUNDO    
    delay(1000);
    asm volatile ("jmp 0");   
  }

  if(rtc.lostPower()){       
    // CASO ELE ESTEJA DESATUALIZADO
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Tempo em que foi compilado
    //rtc.adjust(DateTime(1999, 05, 12, 6, 20, 0)); // Tempo manualmente 
  }
  
  // SET DO TRANSMISSOR
  vw_set_tx_pin(RF_pin);
  vw_setup(2000);
  
}

void loop(){
  

  tempoCompensado = millis();

  aux = 0;
  for (int i = 0; i <7; i++){  
    medicoes[i] = (5.0 * analogRead(S_Temp) * 100 / 1024);
    delay(25);
    aux += medicoes[i];
  }
  
  DateTime now = rtc.now();

  *diaSemana    = now.dayOfTheWeek(); // inteiro
  *ano          = now.year();
  *mes          = now.month();
  *dia          = now.day();
  *hora         = now.hour();
  *minuto       = now.minute();
  *segundo      = now.second();
  *umidade1     = map(analogRead(S_Umidade1), 0, 1023, 0, 100);
  *umidade2     = map(analogRead(S_Umidade2), 0, 1023, 0, 100);
  *temperatura  = (int)aux/8;
    
  printAll();
  
  LedControl(1,1,1,100);
  vw_send((uint8_t *)mensagem, sizeof(mensagem));
  vw_wait_tx();
  LedOff();
  
  delay((TEMPO*1000)-(millis() - tempoCompensado));
  
}

void LedControl(bool verm, bool verd, bool azul, int time){
  verm ? digitalWrite(Led_Verm, HIGH) : digitalWrite(Led_Verm, LOW);
  verd ? digitalWrite(Led_Verd, HIGH) : digitalWrite(Led_Verd, LOW);
  azul ? digitalWrite(Led_Azul, HIGH) : digitalWrite(Led_Azul, LOW);
  delay(time);
}

void LedOff(){
  digitalWrite(Led_Verm, LOW);
  digitalWrite(Led_Verd, LOW);
  digitalWrite(Led_Azul, LOW);
}

void printAll(){
  Serial.print(*ano);
  Serial.print("/");
  Serial.print(*mes);
  Serial.print("/");
  Serial.print(*dia);
  Serial.print(" - ");
  Serial.print(daysOfTheWeek[*diaSemana]);
  Serial.print(" - ");
  Serial.print(*segundo);
  Serial.print(":");
  Serial.print(*minuto);
  Serial.print(":");
  Serial.println(*hora);
  Serial.print("Umidade planta 1: ");
  Serial.print(*umidade1);
  Serial.print("% ");
  Serial.print("\tUmidade planta 2:");
  Serial.print(*umidade2);
  Serial.println("% ");
  Serial.print("Temperatura :");
  Serial.print(*temperatura);
  Serial.println(" C");
}



