#include <SPI.h>

#define WRITE_REGISTER 0x01 
#define WRITE 0x02
#define READ 0x03

#define SEQUENTIAL_MODE 0x40

#define MOSI 11
#define MISO 12
#define CLK  13
#define SS0  10


void initRAM( uint8_t csPin) {
  SPI.beginTransaction( SPISettings(20000000, MSBFIRST, SPI_MODE1) );
  digitalWrite(csPin, LOW);  
  SPI.transfer(WRITE_REGISTER);       
  SPI.transfer(SEQUENTIAL_MODE);
  digitalWrite(csPin, HIGH);      
  SPI.endTransaction();
}

void writeSPIByte(uint8_t csPin, unsigned long adr, uint8_t dataByte) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  digitalWrite(csPin, LOW);                      // select SPI Ram
  SPI.transfer( WRITE );                         
  SPI.transfer((uint8_t)((adr >> 16) & 0xFF));
  SPI.transfer((uint8_t)((adr >> 8) & 0xFF) );
  SPI.transfer((uint8_t) (adr & 0xFF)       );
  SPI.transfer(dataByte);
  digitalWrite(csPin, HIGH);                      // DeSelect SPI ram chip
  SPI.endTransaction();
}

void writeSPIWord(uint8_t csPin, unsigned long adr, uint16_t dataWord) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  digitalWrite(csPin, LOW);
  SPI.transfer( WRITE ); 
  SPI.transfer((uint8_t)((adr >> 16) & 0xFF));
  SPI.transfer((uint8_t)((adr >> 8) & 0xFF));
  SPI.transfer((uint8_t)(adr & 0xFF));
  SPI.transfer16(dataWord);
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();
}

uint8_t readSPIByte(uint8_t csPin, unsigned long adr) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  digitalWrite(csPin, LOW); 
  SPI.transfer( READ ); 
  SPI.transfer((uint8_t)((adr >> 16) & 0xFF));
  SPI.transfer((uint8_t)((adr >> 8) & 0xFF));
  SPI.transfer((uint8_t)(adr & 0xFF));
  uint8_t result = SPI.transfer(0);
  digitalWrite(csPin, HIGH); 
  SPI.endTransaction();
  return result;
}

uint16_t readSPIWord(uint8_t csPin, unsigned long adr) {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  digitalWrite(csPin, LOW);
  SPI.transfer( READ ); // COMANDO READ 
  SPI.transfer((uint8_t)((adr >> 16) & 0xFF));
  SPI.transfer((uint8_t)((adr >> 8) & 0xFF));
  SPI.transfer((uint8_t)( adr & 0xFF) );
  uint16_t result = SPI.transfer16(0);
  digitalWrite(csPin, HIGH);
  SPI.endTransaction();
  return result;
}

void setup() {
  Serial.begin(9600);
  Serial.print("Abracadabra!");
  
  digitalWrite(SS0, HIGH); pinMode(SS0, OUTPUT);

  SPI.begin();
  initRAM(SS0);

  unsigned long adr;
  Serial.println("Write out as Bytes:");
  for (adr = 0; adr < 32; adr++ ){
    writeSPIByte(SS0, adr, (byte) 0xAB );
    Serial.println( adr, HEX);
  }

  Serial.println("Read out as Words:");
  for (adr = 0; adr < 32; adr++) {
    uint16_t a = readSPIByte(SS0, adr);
    Serial.println(a, HEX);
  }
}

void loop() {}
