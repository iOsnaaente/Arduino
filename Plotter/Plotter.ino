

void setup(){

  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  
  Serial.begin(9600);

  analogWrite(6, (int)255/2);
  
}

void loop(){
  
  Serial.println(digitalRead(7));
  
}
