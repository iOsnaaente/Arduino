#include <Servo.h>

#define CM 28 // Constante divisor de Centimetros
#define IN 72 // Constante divisor de Polegadas 

const uint8_t VCC  = 11;
const uint8_t trig = 12;
const uint8_t echo = 13;

float lerDistancia(long int overtime){

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long int duracao = pulseIn(echo, HIGH, overtime*1000);
  
  return ( duracao / (CM * 2.0));
}

Servo servito;

void setup() {

  pinMode(VCC , OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);  

  digitalWrite(VCC, HIGH);

  servito.attach(9);

  Serial.begin(9600);
}

void loop(){

  for(int angle = 0; angle < 180; angle++){
    
    servito.write(angle);

    Serial.print(angle);
    Serial.print(",");
    Serial.println( (int)(lerDistancia(100)*100) );
    
    delay(25);
  }

}
