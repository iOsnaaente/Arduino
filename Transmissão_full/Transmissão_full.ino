/*
* Autor:         Sampaio. Bruno Gabriel
*                Estudante de Engenharia de controle e automaçao - UFSM
*
* Contato:       sampaiobruno1205@gmail.com
*
* Descriçao:     Teste de uma comunicaço Full-Duplex
*                Conexo entre duas pontas que podem enviar e receber dados ao mesmo tempo
*
* Data:          04/06/2020
* 
* Dispositivos:  Par de sensores RF 433MHz 
*                Transmissor: FS1000A
*                Receptor:    MX-RM-05
* 
*/

// Para usar a tranmissão RF 433Mhz
#include <VirtualWire.h>

const int transmissor_pino = 11;
const int receptor_pino    = 12;                  

uint8_t *respostaFAIL = "FAILL!";               // Retransmissão no caso de mensagem com FCS errado (FAILL)
uint8_t *respostaOK   = "OK!";                  // Resposta ao tranmissor que o FCS esta correto (OK) 

#define MAX_BUFF 32                             // Seta o número máximo de bytes para serem enviados 

static uint8_t buff_len = MAX_BUFF;             // Tamanho do buffer
static uint8_t buff[MAX_BUFF];                  // Criação de um Buffer para receber as mensagens do tranmissor

static uint8_t *mensagem;

void recebeMensagem(int tempo_escuta);
void enviaMensagem(uint8_t *mensagem);
void send_server(uint8_t * COD);

void setup() {

  //Iniciar as configurações de transmissão e recepção RF
  vw_set_tx_pin(transmissor_pino);              // Pino transmissor RF
  vw_set_rx_pin(receptor_pino);                 // Pino de Recepção RF
  vw_setup(2000);                               // Bits por segundo
  
  delay(10);

  vw_rx_start();                                // Inicia a recepção de dados RX (Receive)
  Serial.begin(9600);
}


void loop() {
  mensagem = "Hello World!!";
  enviaMensagem(mensagem);
  recebeMensagem(1000);                         // Verifica se há mensagem por 1segundo
  delay(1);
}


/// Função de envio das mensagens
/// \param mensagem Mensagens que serão enviadas no formato (uint8_t *)
void enviaMensagem(uint8_t *mensagem){
  if (vx_rx_active())                           // Se há alguma mensagem sendo recibida
    vw_rx_wait();                               // Esperamos ela terminar de ser escutada
  
  send_server(mensagem);                        // Iniciamos o processo de envio de mensagem
  vw_tx_wait();

  // Conexão simplex no momento -> Uma só direção 
  /*
  buff = NULL;
  recebeMensagem(100);
  if (buff == NULL)
    enviaMensagem(mensagem);
  else
    // RETORNO DA RESPOSTA OK
  */

}

/// Função que verifica periodicamente a existência de uma nova mensagem
/// \param tempo_escuta tempo máximo em milissegundos que o sistema vai ficar escutando
void recebeMensagem(int tempo_escuta){
  if(vx_tx_active()){                           // Se o Tx estiver transmitindo, esperar terminar
    vw_wait_tx();                                 
    if(vw_wait_rx_max(tempo_escuta))            // Espera Xms ou até receber uma mensagem 
      recebeMensagem(tempo_escuta);
  }else{
    if (vw_have_message())                      // Se tem uma mensagem esperando, escute-a
      vw_get_message(buff, &buff_len) ?         /* Retorna True se FCS esta correto (Frame Check Sequence)*/
        send_server(respostaOK):                /* FCS errado (Mensagem com erro = Retransmissão)*/
        send_server(respostaFAIL);
    else
      if(vw_wait_rx_max(tempo_escuta))          // Espera Xms ou até receber uma mensagem 
        recebeMensagem(tempo_escuta);           // Chama a função recursivamente
  }
}

/// Envia os simbolos, parando de escutar brevemente, para que ele não se escute
/// \param COD mensagem no formato (uint8_t *) 
void send_server(uint8_t * COD){
  vw_rx_stop();                                 // Para de escutar no seu receptor
  vw_send(COD, sizeof(COD));                    // Envia o frame
  vw_wait_tx();                                 // Aguarda envio
  vw_rx_start();                                // Volta a escutar  
}