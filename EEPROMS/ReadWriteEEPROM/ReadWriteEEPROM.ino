#include <Wire.h> 

#define ADDR_EEPROM 0x57 

void writeEEPROM( int deviceAddr, unsigned int addr, byte data);
byte readEEPROM( int deviceAddr, unsigned int addr );

void setup() {
  Serial.begin(9600);
  Wire.begin();

  unsigned int addr = 0x00; 

  Serial.print("Escrito na EEPROM no endereço 0x00 : "); 
  Serial.println( 0x12 , HEX );

  //writeEEPROM( ADDR_EEPROM, addr, 0x12 );
  
  Serial.print("Echo da EEPROM no endereço 0x00 : "); 
  Serial.println( readEEPROM(ADDR_EEPROM, addr), HEX ); 

}

void loop() {
  
  
}


void writeEEPROM( int deviceAddr, unsigned int addr, byte data){
  Wire.beginTransmission(deviceAddr);
  Wire.write( (int) addr >> 8    );
  Wire.write( (int) addr &  0xff );
  Wire.write( data );
  Wire.endTransmission();   
}

byte readEEPROM( int deviceAddr, unsigned int addr ){
  byte data = 0xff; 
  Wire.beginTransmission( deviceAddr );
  Wire.write( (int) addr >> 8    );
  Wire.write( (int) addr &  0xff );
  Wire.endTransmission();

  Wire.requestFrom( deviceAddr, 1);

  if ( Wire.available() )
    data = Wire.read();

  return data;
}
