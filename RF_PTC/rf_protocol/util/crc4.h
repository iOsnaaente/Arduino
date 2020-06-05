/// This util will be an example of CRC code for 
/// Codefy an 8bits algorithm of data 
/// 
/// To define this CRC size I used the data size
/// I will tramismitte a 8bit character per time
/// So I need a small CRC code (1bit would be enouth)
///
///
/// The parameter of CRC used was the 4 bits ITU
/// with the generator G(x) = x⁴ + x¹ + 1  (0001 0011)
///
///
/// Leason of Industrial Networks 
///
/// Author: Bruno Gabriel

#include<stdint.h>
#include<stdio.h>

#define GEN (0x03+(1<<4))           // 0x13 

uint8_t CRC4_Codefy(uint16_t crc){

    crc <<= 4;                      // if data was 0x00ff now it's 0x0ff0

    for (int i=12; i>=4; i--)
        if (crc & (1<<i) ) 
            crc ^= (GEN << i-4 );    // 0001 0000 0000 0000

    return (uint8_t) crc;    
}

bool CRC4_Decodefy(uint16_t data){

    uint8_t crc = data & 0x000f ;   // if data was 0x0fff the crc will be 0x000f
    data &= 0xfff0 ;                // the data without the crc pre calculated to compare

    for (int i=12; i>=4; i--)
        if (data & (1<<i) ) 
            data ^= (GEN << i-4 );   // 0001 0000 0000 0000
   
   // if the crc calculated is equal to the crc received returns true, else false  
   return data == crc ? true : false;}


