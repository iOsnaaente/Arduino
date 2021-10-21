/*
  GY-271 Compass
  modified on 02 Out 2021
  by Bruno Sampaio - UFSM @ ECA
*/

#include <Wire.h>
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
}

int x, y, z;

void loop() {
  compass.read();
  
  x = (float)compass.getX();
  y = (float)compass.getY();
  z = (float)compass.getZ();
  
  union converter {
    float ac[3] = { x, y, z };
    byte bc[12];
  } converted;
  
  for ( int i = 0; i < 12; i++) 
    Serial.write( converted.bc[ i ]);

  delay(100);
}
