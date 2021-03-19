#include <Helios.h>

#define BUFF 18

double ddLongitude = -54.89831672609559;
double ddLatitude = -29.165307659422155;

bool receiveComplete = false;

byte bytes_time[BUFF];
int num = 0;

union data_struct {
  byte inputStr[BUFF];
  struct datetime {
    int iiYear;
    int iiMonth;
    int iiDay;
    int ddHours;
    int ddMinutes;
    int ddSeconds;
  } date;
};

Helios heliosCalc;

void setup() {
  Serial.begin(115200);
}


void loop() {

  data_struct info;

  if (receiveComplete) {
    for (int i = 0; i < BUFF; i++) {
      info.inputStr[i] = bytes_time[i];
      bytes_time[i] = 0;
    }

    heliosCalc.calcSunPos(
      info.date.iiYear,
      info.date.iiMonth,
      info.date.iiDay,
      (double)info.date.ddHours,
      (double)info.date.ddMinutes,
      (double)info.date.ddSeconds,
      ddLongitude,
      ddLatitude
    );

    union az{
      long int var_print = (long int) (heliosCalc.dAzimuth*100); 
      byte    byte_print[4];
    }azi;
    
    for ( int i = 0; i < sizeof(long int); i++)
      Serial.write( azi.byte_print[i]  ); 

    
    union ze{
      long int var_print = (long int)(heliosCalc.dZenithAngle *100 );
      byte    byte_print[4];
    }zen;
    
    for ( int i = 0; i < sizeof(long int); i++)
      Serial.write( zen.byte_print[i]  );

    
    union al{
      long int var_print = (long int)(heliosCalc.dElevation *100 );
      byte    byte_print[4];
    }alt;
    
    for ( int i = 0; i < sizeof(long int); i++)
      Serial.write( alt.byte_print[i]  );
 

    receiveComplete = false;
  }

}

void serialEvent() {
  while ( Serial.available() || num > 12) {
    char inChar = (byte)Serial.read();
    bytes_time[num] = inChar;
    num++;
    if (inChar == '\\') {
      receiveComplete = true;
      num = 0;
    }
  }
}
