/*
  Helios.h - Library for calculating the solar
  position.
  Created by Hannes Hassler, February, 2011.  
*/
#ifndef DS1107H_h
#define DS1107H_h

#include "WProgram.h"


class DS1107H
{
  public:
    DS1107H();   
    byte decToBcd(byte val);
    byte bcdToDec(byte val);
    
    byte second;        // 0-59
    byte minute;       // 0-59
    byte hour;          // 1-23
    byte dayOfWeek;     // 1-7
    byte dayOfMonth;    // 1-28/29/30/31
    byte month;         // 1-12
    byte year;
    
    void setDate(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year);
	
   void getDate();	  
  	
  
   
};




#endif
