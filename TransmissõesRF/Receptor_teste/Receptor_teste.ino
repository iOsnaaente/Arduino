// Para usar a tranmissão RF 433Mhz
#include <VirtualWire.h>

const int receiver_pin    = 11;      

#define MAX_BUFF 31

uint8_t BUFF_LEN = MAX_BUFF;
uint8_t BUFF[MAX_BUFF];

void setup() {
 
  //Iniciar as configurações de transmissão e recepção RF
  vw_set_rx_pin(receiver_pin);        // Pino de Tranmissão RF
  vw_setup(2000);                     // Bits por segundo
  
  delay(10);

  vw_rx_start();                     // Inicia a recepção de dados RX (Receive) PLL
  Serial.begin(9600);
}


void loop() {

  if (vw_have_message()){  
    if(vw_get_message(BUFF, &BUFF_LEN)){
      for (int i=0; i<BUFF_LEN; i++){
        Serial.begin(BUFF[i], HEX);
      }
      Serial.println();
    }
  }
  delay(10);
}

