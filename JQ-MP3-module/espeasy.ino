#include "_Plugin_Helper.h"
//#######################################################################################################
//############################# Plugin 065: P065_DFR0299_MP3 ############################################
//#######################################################################################################

// ESPEasy Plugin to controls a MP3-player-module DFPlayer-Mini SKU:DFR0299
// written by Jochen Krapf (jk@nerd2nerd.org)

// Important! The module WTV020-SD look similar to the module DFPlayer-Mini but is NOT pin nor command compatible!

// Commands:
// play,<track>        Plays the n-th track 1...3000 on SD-card in root folder. The track number is the physical order - not the order displayed in file explorer!
// stop                Stops actual playing sound
// vol,<volume>        Set volume level 1...30
// eq,<type>           Set the equalizer type 0=Normal, 1=Pop, 2=Rock, 3=Jazz, 4=classic, 5=Base

// Circuit wiring
// 1st-GPIO -> ESP TX to module RX [Pin2]
// 5 V to module VCC [Pin1] (can be more than 100 mA) Note: Use a blocking capacitor to stabilise VCC
// GND to module GND [Pin7+Pin10]
// Speaker to module SPK_1 and SPK_2 [Pin6,Pin8] (not to GND!) Note: If speaker has to low impedance, use a resistor (like 33 Ohm) in line to speaker
// (optional) module BUSY [Pin16] to LED driver (3.3 V on idle, 0 V on playing)
// All other pins unconnected

// Note: Notification sounds with Creative Commons Attribution license: https://notificationsounds.com/

// Datasheet: https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299



#define PLUGIN_065
#define PLUGIN_ID_065         65
#define PLUGIN_NAME_065       "Notify - DFPlayer-Mini MP3 [TESTING]"
#define PLUGIN_VALUENAME1_065 ""

#include <ESPeasySerial.h>



ESPeasySerial* P065_easySerial = NULL;


boolean Plugin_065(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_065;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = Sensor_VType::SENSOR_TYPE_NONE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 0;
        Device[deviceCount].SendDataOption = false;
        Device[deviceCount].TimerOption = false;
        Device[deviceCount].GlobalSyncOption = false;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_065);
        break;
      }

      case PLUGIN_GET_DEVICEGPIONAMES:
        {
          event->String1 = formatGpioName_TX(false);
          break;
        }

    case PLUGIN_WEBFORM_LOAD:
      {
          addFormNumericBox(F("Volume"), F("volume"), PCONFIG(0), 1, 30);

          success = true;
          break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        PCONFIG(0) = getFormItemInt(F("volume"));

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        #pragma GCC diagnostic push
        //note: we cant fix this, its a upstream bug.
        #pragma GCC diagnostic warning "-Wdelete-non-virtual-dtor"
        if (P065_easySerial)
          delete P065_easySerial;
        #pragma GCC diagnostic pop


        P065_easySerial = new (std::nothrow) ESPeasySerial(static_cast<ESPEasySerialPort>(CONFIG_PORT), -1, CONFIG_PIN1);   // no RX, only TX
        if (P065_easySerial != nullptr) {
          P065_easySerial->begin(9600);
          Plugin_065_SetVol(PCONFIG(0));   // set default volume

          success = true;
        }
        break;
      }

    case PLUGIN_WRITE:
      {
        if (!P065_easySerial)
          break;

        String command = parseString(string, 1);
        String param = parseString(string, 2);

        if (command == F("play"))
        {
          int track;
          if (validIntFromString(param, track)) {
            Plugin_065_Play(track);
            if (loglevelActiveFor(LOG_LEVEL_INFO)) {
              String log = F("MP3  : play=");
              log += track;
              addLog(LOG_LEVEL_INFO, log);
            }
          }
          success = true;
        }

        if (command == F("stop"))
        {
          String log = F("MP3  : stop");

          Plugin_065_SendCmd(0x0E, 0);

          addLog(LOG_LEVEL_INFO, log);
          success = true;
        }

        if (command == F("vol"))
        {
          String log = F("MP3  : vol=");

          int8_t vol = param.toInt();
          if (vol == 0) vol = 30;
          PCONFIG(0) = vol;
          Plugin_065_SetVol(vol);
          log += vol;

          addLog(LOG_LEVEL_INFO, log);
          success = true;
        }

        if (command == F("eq"))
        {
          String log = F("MP3  : eq=");

          int8_t eq = param.toInt();
          Plugin_065_SetEQ(eq);
          log += eq;

          addLog(LOG_LEVEL_INFO, log);
          success = true;
        }

        break;
      }
  }
  return success;
}


void Plugin_065_Play(uint16_t track)
{
  Plugin_065_SendCmd(0x03, track);
}

void Plugin_065_SetVol(int8_t vol)
{
  if (vol < 1) vol = 1;
  if (vol > 30) vol = 30;
  Plugin_065_SendCmd(0x06, vol);
}

void Plugin_065_SetEQ(int8_t eq)
{
  if (eq < 0) eq = 0;
  if (eq > 5) eq = 5;
  Plugin_065_SendCmd(0x07, eq);
}


// Comando com mais de 1 argumento 
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
    uint16_t CHECKSUM = CMD_BEGIN + cmd + arg1 + arg2 + length;
    
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
    buffer[i++] = CHECKSUM >> 8 ;
    
    // Avalia o length 
    P065_easySerial->write(buffer, 4+length );   //Send the byte array

    String log = F("MP3  : Send Cmd ");
    for (byte i=0; i<10; i++)
    {
    log += String(buffer[i], 16);
    log += ' ';
    }
    addLog(LOG_LEVEL_DEBUG, log);
}
