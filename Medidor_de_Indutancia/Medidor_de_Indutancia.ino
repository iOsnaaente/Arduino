/*
  -=-=-=-=-=-[Projeto Medidor de Indutância com a Placa Roduino ou Arduino UNO]-=-=-=-=-=-
  Se você não deseja mostrar a medição no Monitor Serial da IDE, basta exluir os códigos referentes a isso.
  Deixe apenas as linhas de código que apresentam o resultado da leitura de Indutância no display LCD.
  Neste projeto utilizamos um Display LCD do tipo I2C.
  Projeto Modificado e Adaptado por: Rodrigo Costa do Blog - EletronicaParaTodos.com
  ...
  Código Original de: electronoobs.com
*/

#include <Arduino.h>

//13 é a entrada do circuito (conecta-se ao resistor de 150 ohm), 11 é a saída do comparador/amplificador operacional.
double pulse, frequency, capacitance, inductance;
void setup(){
  Serial.begin(115200);
  pinMode(11, INPUT);
  pinMode(13, OUTPUT);
  Serial.println("Iniciando Medição...");
  delay(200);
}
void loop(){
  digitalWrite(13, HIGH);
  delay(5);//dê algum tempo para carregar o indutor.
  digitalWrite(13,LOW);
  delayMicroseconds(100); //verifique se a ressonância é medida.
  pulse = pulseIn(11,HIGH,5000);//retorna 0 se o limite de tempo chegar
  if(pulse > 0.1){ //se o tempo limite não ocorrer e dessa forma proceder a leitura:
    
    
  //#error insira o valor de capacitância utilizada aqui. Atualmente estamos utilizando 2uF. Exclua esta linha depois de inserir sua própria capacitância!
  capacitance = 2.E-6; // - insira o valor da capacitância utilizada aqui. Atualmente setamos 2uF!
  
  
  frequency = 1.E6/(2*pulse);
  inductance = 1./(capacitance*frequency*frequency*4.*3.14159*3.14159);//um dos meus professores me ensinou a fazer quadrados como na fórmula.
  inductance *= 1E6; //perceba que isso é o mesmo que dizer indutância = indutância * 1E6

  //Impressão Serial (Monitor Serial da IDE)
  Serial.print("Alto para uS:");
  Serial.print( pulse );
  Serial.print("\tFrequencia Hz:");
  Serial.print( frequency );
  Serial.print("\tIndutancia uH:");
  Serial.println( inductance );
  delay(1000);

  }
}
