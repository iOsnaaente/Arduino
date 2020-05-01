/// THIS IS A COPY OF VirtualWire.h MADE BY Mike McCauley 
/// $Id: VirtualWire.h,v 1.6 2013/02/14 22:02:11 mikem Exp mikem $
///
/// VirtualWire.h
///
/// To understand the concepts used in the lib: VirtualWire 
/// I decide to recreate this lib with my owns concepts 
/// It's a copy of VW lib available in the internet but with no complexity
/// 
/// This lib will be used to communicate a RF module with a Arduino UNO
/// The concepts used to do that was learned in the class, so, it will be a test
/// to compare my understood of the theory aborded in classroom 


// A native lib of C 
#include <WProgram.h>

// The header that we do 
#include "rf_ptc.h"

// The CRC Extern math code 
#include <util/crc4.h>

static int  tx_pin          = NULL ;
static int  rx_pin          = NULL ;
static int  ptt_pint        = NULL ;
static bool ptt_inverted    = false;
static int  speed           = MAX_SPEED_BPS ;


void set_tx_pin(uint8_t pin = NULL){
    if (pin)
        tx_pin = pin;
    else 
        tx_pin = 10;
}

void set_rx_pin(uint8_t pin = NULL){
    if (pin)
        rx_pin = pin;
    else
        rx_pin = 9;
}

/// ptt (press to talk) used as a flag to made the transmitter enable
void set_ptt_pin(uint8_t pin = NULL){
    if (pin)
        ptt_pin = pin;
    else 
        ptt_pin = 11; 
}

/// The ptt pin goes to HIGH when the transmitter is enable
/// If the ptt_invert is true, it's make goes to LOW
void set_ptt_inverted(bool inverted){
    ptt_inverted = inverted;
}

/// Beging the communication with the communication's speed (bps)
void setup_communication(uint16_t speed){
    speed = speed;
}

/// Must be called before any message be received
/// Inicialize the Phase Locked Loop (PLL)
void rx_start(){

    

}

/// Stop the PLL
/// To receive another message, must call the rx_start again
void rx_stop();

/// Returns the state of the transmitter
/// \return true if tramitter is active : false else  
uint8_t tx_active();

/// Wait for the transmitter is idle
/// Returns after 
void wait_tx();

/// Wait until a message is available 
/// After returns 
void wait_rx();

/// Wait the message for a limited time
/// \param millis Time maximum of waiting
/// \return true if available : false if not  
uint8_t wait_rx_max(unsigned long millis);

/// Send the message
/// \param buf Pointer to data, the data that will be sent
/// \param len Number of octetes (byte) of data -------------------------------------------------------------- can be exchanged for sizeof(buf) ???  
/// \return true if acepted : false if was too long to send (message > MAX_MESSAGE_LEN - 3)
uint8_t send_messagem(uint8_t *buf, uint8_t len);

/// Returns true if have an unread message
/// \return True if a message is available to read : false if not 
uint8_t have_message();

/// If a message is available, it will copy it 
/// \param buf Pointer to the location to save the data receive
/// \param len Available space in buf. Will be set to the actual number of octets read
/// \return true if there was a message and the checksum is good  
uint8_t get_message(uint8_t *buf, uint8_t *len );


/// Another functions that arent in the rf_ptc.h 
/// Phase Locked Loop -> to syncronize the trasmitter and the receiver

void pll(){

}