void setup(){

  pinMode(6, OUTPUT);              // Liga o pino 6
  pinMode(7, INPUT);               // No pino 7 
  
  analogWrite(6, (int)255/2);      // Ativa PWM dutycilce de 50%
  
  Serial.begin(9600);
  delay(1);
}

void loop(){
  Serial.println(digitalRead(7));   // Ativar o plotter para testar o osciloscopio
  delay(1);
}
