// Para usar a tranmissão RF 433Mhz
#include <VirtualWire.h>

const int transmit_pin    = 11;      

char mensagem[4];
int length = sizeof(mensagem);

uint8_t Seg = 0;
uint8_t Min = 0;
uint8_t Hor = 0;

void setup() {
 
  //Iniciar as configurações de transmissão e recepção RF
  vw_set_tx_pin(transmit_pin);        // Pino de Tranmissão RF
  vw_setup(2000);                     // Bits por segundo
  
  delay(10);

  //vw_rx_start();                    // Inicia a recepção de dados RX (Receive)
  Serial.begin(9600);
}


void loop() {

  delay(1000);
  Seg++;

  // Contagem do tempo
  if (Seg == 60) {    
    Min++;
    if (Min >= 60){
      Min = 0;
      Hor++;
    }
    Seg = 0;                          //Zera a variável i para uma nova contagem
  }

  mensagem[0] = 0xaa;                 // 1010 1010 Preâmbulo 
  mensagem[1] = Seg;
  mensagem[2] = Min;
  mensagem[3] = Hor; 

  vw_send((uint8_t *)mensagem, length);
  vw_wait_tx();
}

