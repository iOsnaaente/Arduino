#include <VirtualWire.h>

const int RECPT = 12;

#define BUFF_MAX_LEN  12

uint8_t buff_max_len = BUFF_MAX_LEN;
uint8_t buff[BUFF_MAX_LEN];

byte ack;
byte mstat;
byte mx;
byte my;

void setup(){
  Serial.begin(9600);
  vw_set_rx_pin(RECPT);
  vw_setup(2000);
  vw_rx_start();

}


void loop(){

  vw_wait_rx_max(200);
  if (vw_get_message(buff,&buff_max_len)){
    ack   = buff[0];
    mstat = buff[1];
    mx    = buff[2];
    my    = buff[3];

    printMouse(ack, mstat, mx, my);
  }

  delay(1);
}

void printMouse(byte ack, byte mstat, byte mx, byte my){
  Serial.print("Ack=");  
  Serial.print(ack, HEX);
  Serial.print("\tIN=");
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.println();  
}









