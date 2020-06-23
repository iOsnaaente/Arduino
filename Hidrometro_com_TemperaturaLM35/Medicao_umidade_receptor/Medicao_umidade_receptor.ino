// RECEPTOR RF 433MHz
#include <VirtualWire.h>


#define RX_pin 11


uint8_t BUFF[20];
byte    lenBUFF = sizeof(BUFF);


void setup(){
  
  Serial.begin(9600);
  
  vw_set_rx_pin(RX_pin);
  vw_setup(2000);
  
  vw_rx_start();
  
}


void loop(){
  
 vw_wait_rx_max(1000);
 
 if(vw_have_message()){
   if(vw_get_message(BUFF, &lenBUFF)){
     Serial.print("0 ");
     for (int i=0; i<lenBUFF; i++){
       Serial.print(BUFF[i]);
       Serial.print(" ");
     }
     Serial.println();     
   }
 }
 
 delay(1);    
}
