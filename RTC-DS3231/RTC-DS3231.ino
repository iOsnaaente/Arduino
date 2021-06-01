// IMPORTAÇÕES
#include <RTClib.h>
#include <Wire.h>


// OBJETOS
RTC_DS3231 Clock;
DateTime   Date ;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Date = Clock.now();

  int year   =  Date.year();
  int month  =  Date.month();
  int day    =  Date.day();
  int hour   =  Date.hour() + 3;
  int minute =  Date.minute();
  int second =  Date.second();

  Serial.print( Date.year() ); 
  Serial.print("/ "); 
  Serial.print( Date.month() );
  Serial.print("/ "); 
  Serial.print( Date.day() );
  Serial.print("\t\t"); 
  Serial.print( Date.hour() );
  Serial.print(":"); 
  Serial.print( Date.minute() );
  Serial.print(":"); 
  Serial.println( Date.second() ); 

  delay(1000);
}
