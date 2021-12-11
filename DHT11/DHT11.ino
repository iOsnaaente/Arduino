#include "TinyDHT.h"

const int DHT11_pin =  A1 ;

// Pino e tipo (DHT11) 
DHT dht( DHT11_pin, DHT11 );


void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando o DHT11" );
  dht.begin();  
}


void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if ( isnan(t) || isnan(h) ){
    Serial.println("Sensor não conseguiu ler a temperatura e umidade");
  
  }else{
    Serial.print("Umidade(%): ");
    Serial.print(h);
    Serial.print("\t");
    Serial.print("Temperatura(ºC): ");
    Serial.println(t);
  }
  
}
