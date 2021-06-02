#define WRITE_ENABLE            0x06 // Address Write Enable
#define WRITE_DISABLE           0x04 // Address Write Disable
#define CHIP_ERASE              0xc7 // Address Chip Erase
#define READ_STATUS             0x05 // Address Read Status
#define READ_DATA               0x03 // Address Read Data
#define PAGE_PROGRAM_STAT       0x02 // Address Status Page Program
#define CHIP_COMMAND_ID         0x9f // Address Status Read Id

#include <SPI.h> 

// ESPERA O 25Q32 RETORNAR UM VALOR CONFIRMANDO QUE ESTA DIPONÍVEL
void not_busy(){
  digitalWrite( SS, HIGH );
  digitalWrite( SS, LOW  );
  SPI.transfer( READ_STATUS );
  while ( SPI.transfer( 0 ) & 1 );
  digitalWrite( SS, HIGH );
}

/// RETORNA AS INFORMAÇÕES DE CHIP DO 25Q32
void chipInformation( byte *ID, byte *memoryType, byte *capacity){
  digitalWrite( SS, HIGH );
  digitalWrite( SS, LOW  );
  SPI.transfer( CHIP_COMMAND_ID );
  *ID         = SPI.transfer( 0 );
  *memoryType = SPI.transfer( 0 );
  *capacity   = SPI.transfer( 0 );
  digitalWrite( SS, HIGH );
  not_busy();
}

/// APAGA TODOS OS BYTES GRAVADOS 
void chipErase(){
  Serial.print("Comando CHIP_ERASE\n");
  digitalWrite( SS, HIGH );
  digitalWrite( SS, LOW  );
  SPI.transfer( WRITE_ENABLE );
  digitalWrite( SS, HIGH );
  digitalWrite( SS, LOW  );
  SPI.transfer( CHIP_ERASE );
  digitalWrite( SS, HIGH );
  not_busy();
}

/// PRINTA OS 256 BYTES DA PÁGINA 
void printPage( byte *page ){
  char buf[10];
  for (int i = 0; i < 16; i++){
    for (int j = 0; j < 16; j++){
      sprintf( buf, "%02x", page[ i*16 + j]);
      Serial.print(buf);
    }
    Serial.println();
  }
}

/// CONSIDERANDO UMA PÁGINA COM 256 BYTES 
void readPage( unsigned int page, byte *page_buff ){
  char buff[255];
  sprintf( buff, "Lendo a página (%04xh)\n", page);
  Serial.print( buff ); 
 
  digitalWrite( SS, HIGH );
  digitalWrite( SS, LOW  );
  SPI.transfer( READ_DATA   );
  SPI.transfer( page >> 8   );
  SPI.transfer( page & 0xff );
  SPI.transfer( 0 ); 
  for ( int i = 0; i < 256; i++)
    page_buff[i] = SPI.transfer( 0 ); 
  digitalWrite( SS, HIGH );
  not_busy(); 
}

/// ESCREVE 256 BYTES EM UMA PÁGINA
void writePage( unsigned int page, byte *data_buff ){
  char buff[255];
  sprintf( buff, "Escrevendo na página (%04xh)\n", page);
  Serial.print( buff ); 
  digitalWrite( SS, HIGH     );
  digitalWrite( SS, LOW      );  
  SPI.transfer( WRITE_ENABLE );
  digitalWrite( SS, HIGH     );
  digitalWrite( SS, LOW      );  
  SPI.transfer( PAGE_PROGRAM_STAT);
  SPI.transfer( page >> 8   );
  SPI.transfer( page & 0xff );
  SPI.transfer( 0 ); 
  for ( int i = 0; i < 256; i++)
    SPI.transfer( data_buff[i] );
  digitalWrite( SS, HIGH );
  not_busy();
}

/// ESCREVE UM BYTE DATA NA POSIÇÃO OFFSET DENTRO DA PAGINA PAGE 
void writeOneByte( unsigned int page, byte offset, byte data ){
  byte buff[256];
  readPage( page, buff );
  buff[ offset ] = data;
  writePage( page, buff );
}

void setup() {
  SPI.begin();
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
 
  Serial.begin(9600);

  // Gerando valores de 0 até 255 no buffer
  byte buff[256];
  for ( byte data = 0; data < 255; data ++)
    buff[data] = data; 
  writePage( 0, buff );
   
  // Para escrever apenas um Byte no buffer
  //writeOneByte( 0, 0, 0xAB );

  // Ler a página e salvar no BUFF 
  readPage( 0, buff );
  for ( int j=0; j<16; j++ ){
    for ( int i=0; i<16; i++){
      Serial.print( buff[ j*i + i ],HEX );
    }
    Serial.println();
  }

  delay(1);
}


void loop() {}
