#include "crc4.h"

int main() {
    
    uint8_t teste1 = 0xf1; // 1111 0001 
    uint8_t teste2 = 0x88; // 1000 1000

    uint8_t crc1 = CRC4_Codefy((uint16_t)teste1);

    uint16_t data = (teste1 << 4) + (crc1 & 0x000f);

    printf("%d \n", crc1);

    bool verify = CRC4_Decodefy(data);

    printf("%d \n", verify);

    uint8_t crc2 = CRC4_Codefy(teste2);
    uint16_t data2 = (teste2<<4) + (crc1 & 0xf);

    printf("%d \n", CRC4_Decodefy(data2));

    return 0;
}