#define DATAOUT     11 // MOSI
#define DATAIN      12 // MISO
#define SPICLOCK    13 // sck
#define SLAVESELECT 10 // ss

#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

byte eeprom_output_data;
byte eeprom_input_data = 0;

byte clr;
int address = 0;

//data buffer
byte buff [128];

void fill_buffer(){
  for (int I = 0; I < 128; I++)
    buff[I] = I;
}

char spi_transfer(volatile char data){
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1 << SPIF)))   // Wait the end of the transmission
  return SPDR;                    // return the received byte
}

void setup(){

  Serial.begin(9600);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(SLAVESELECT, OUTPUT);

  digitalWrite(SLAVESELECT, HIGH); //disable device
  
  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (fastest)
  SPCR = (1 << SPE) | (1 << MSTR);
  clr = SPSR;
  clr = SPDR;
  delay(10);

  fill_buffer();

  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(WREN);
  digitalWrite(SLAVESELECT, HIGH);
  delay(10);
  
  address = 0;
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(WRITE); 
  spi_transfer((char)(address >> 8)); //send MSByte address first
  spi_transfer((char)(address));      //send LSByte address
  for (int i = 0; i < 128; i++)
    spi_transfer(buff[i]);          //write data byte
  digitalWrite(SLAVESELECT, HIGH);    //release chip
  delay(1000);

}

byte read_eeprom(int EEPROM_address){
  int data;
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(READ);                         //transmit read opcode
  spi_transfer((char)(EEPROM_address >> 8));  //send MSByte address first
  spi_transfer((char)(EEPROM_address));       //send LSByte address
  data = spi_transfer(0xFF);                  //get data byte
  digitalWrite(SLAVESELECT, HIGH);            //release chip, signal end transfer
  return data;
}

void loop(){
  eeprom_output_data = read_eeprom(address);
  Serial.println(eeprom_output_data, DEC);
  address++;
  if (address == 128)
    address = 0;
  delay(100); 
}
