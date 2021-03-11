long int tempo_lido; 

int tempo_max = 30000; 

int led_vermelho = 3;
int led_verde = 2;
int pirno = 4;

bool leitura;


void setup() {
  pinMode( led_vermelho, OUTPUT );
  pinMode( led_verde, OUTPUT );
  pinMode( pirno, INPUT );

  Serial.begin(9600);
}

void loop() {

  leitura = digitalRead(pirno);

  if (leitura == 1){
    Serial.println("O sinal do sensor é HIGH, sistema ligado... ");
    tempo_lido = millis();
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_vermelho, LOW);
    
  }else{
    if( millis() - tempo_lido > tempo_max){
      Serial.println( "Tempo limite atingido... Sistema Desligado até a próxima medição HIGH...");  
    
    }else{
      Serial.println("O sinal do sensor é LOW, sistema em espera... ");
      digitalWrite(led_vermelho, HIGH);
      digitalWrite(led_verde, LOW);    
    }
  }
  
  delay(1000); 

}
