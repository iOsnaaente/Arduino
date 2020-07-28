#include <VirtualWire.h>
#include <ps2.h>

const int DATA  = 5;
const int CLOCK = 6;

PS2 mouse(CLOCK, DATA);

byte  mstat;
byte  ack;
byte  mx;
byte  my;

byte msg[4];


void mouse_init(){
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}

const int TRANSMIT = 8;


void setup(){

  digitalWrite(9 , HIGH);
  digitalWrite(10, LOW);

  Serial.begin(9600);
  mouse_init();
  vw_set_tx_pin(TRANSMIT);
  vw_setup(2000);
  delay(10);

}

void loop(){

  mouse.write(0xeb);  

  ack   = mouse.read();      
  mstat = mouse.read();
  mx    = mouse.read();
  my    = mouse.read();

  printMouse(ack, mstat, mx, my);

  msg[0] = ack;
  msg[1] = mstat;
  msg[2] = mx;
  msg[3] = my;

  vw_send((uint8_t *)msg, sizeof(msg));
  vw_wait_tx();

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

