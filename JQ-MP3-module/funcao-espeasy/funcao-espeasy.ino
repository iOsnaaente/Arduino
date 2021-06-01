// Comandos com argumentos nulos 
#define SPECIFY_SONG                 0x07 // 2 bytes - ( Hi lw ) 
#define SWITCH_SPECIFIED_DRIVE       0x0B // 1 byte  - 0 USB 1 SD Card 2 Flash  
#define VOLUME_CONTROL               0x13 // 1 byte  - De 0 a 30 
#define EQ_SETTING                   0x1A // 1 byte  - 
#define SELECT_AUDIO_NOT_PLAYBACK    0x1F // 2 bytes - ( Hi Lw )
#define SPECIFY_DURATION_BACKWARD    0x22 // 2 bytes - ( Hi Lw )
#define SPECIFY_DURATION_FASTFORWARD 0x23 // 2 bytes - ( Hi Lw )

void Plugin_065_SendCmd(byte cmd, int16_t data){
    if (!P065_easySerial)
        return;

    // byte inicial padrão 
    uint8_t CMD_BEGIN = 0xAA;

    uint8_t length = 0;

    // Pega os ultimos 8 bits de data 
    uint8_t arg1 = data ;

    // Pega os primeiros 8 bits de data 
    uint8_t arg2 = data >> 8 ;

    switch( cmd ){         
      case SPECIFY_SONG                 : length = 2; break;
      case SWITCH_SPECIFIED_DRIVE       : length = 1; break;
      case VOLUME_CONTROL               : length = 1; break;        
      case EQ_SETTING                   : length = 1; break;
      case SELECT_AUDIO_NOT_PLAYBACK    : length = 2; break;
      case SPECIFY_DURATION_BACKWARD    : length = 2; break;
      case SPECIFY_DURATION_FASTFORWARD : length = 2; break;
      default:
        if (arg1 != 0)  length ++; 
        if (arg2 != 0)  length ++; 
        break; 
    }

    // Soma de todos os bytes 
    uint8_t CHECKSUM = CMD_BEGIN + cmd + arg1 + arg2 + length;
    
    // Monta o buffer para enviar 
    uint8_t buffer[6];
    uint8_t i = 0; 
    
    buffer[i++] = CMD_BEGIN;
    buffer[i++] = cmd; 
    buffer[i++] = length;

    if (length > 0)
        buffer[i++] = arg1;
    if(length > 1)
        buffer[i++] = arg2;

    // Pega os ultimos bits do checksum 
    buffer[i++] = CHECKSUM;
    
    // Avalia o length 
    P065_easySerial->write(buffer, 4+length );   //Send the byte array

    String log = F("MP3  : Send Cmd ");
    for (byte i=0; i<10; i++){
      log += String(buffer[i], 16);
      log += ' ';
    }
    addLog(LOG_LEVEL_DEBUG, log);
}
