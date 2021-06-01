# include <EEPROM.h>

void setup( void ) {
  Serial.begin(9600); 
}

void loop( void ) {
  if (Serial.available()){
    int comando = Serial.parseInt();
    if (comando == 1){
      int valor = Serial.parseInt();
      EEPROM.write( 0, (byte)(valor) );
      EEPROM.write( 1, (byte)(valor>>8) );
    }
    if (comando == 2){
      union {
        byte read_from[2] = { EEPROM.read(0), EEPROM.read(1)};
        int output;
      } value;
      Serial.println( value.output );
    }
  }
  delay(1000); 
}
