#include <Wire.h>
#include <SFE_MMA8452Q.h>

MMA8452Q acelerometro(0x1C);

void setup() {
  Serial.begin(9600);

  // Sets ODR to 800, 400, 200, 100, 50, 12.5, 6.25, or 1.56 Hz.
  // Sets SCALE to 2g 4g or 8g
  // accel.init(SCALE_2G, ODR_50);
  acelerometro.init( SCALE_2G, ODR_50 );

  Serial.print("Inciando o sistema com SCALE: ");
  Serial.print( SCALE_2G );
  Serial.print( " e ODR: ");
  Serial.println( ODR_50 );
}

void loop() {
  acelerometro.read();
  union converter {
    float ac[3] = { acelerometro.cx, acelerometro.cy, acelerometro.cz };
    byte bc[12];
  } converted;

  for ( int i = 0; i < 12; i++) Serial.write( converted.bc[ i ]);
  Serial.println();
  delay(20);
}

  
