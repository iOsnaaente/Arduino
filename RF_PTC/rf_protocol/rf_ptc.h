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



#ifndef rf_ptc_h
    #define rf_ptc_h

    #include <stdlib.h>

    #if defined(ARDUINO)                //If the board was an Arduino
        #include <Arduino.h>            //Will be include the Arduino Lib
    #else                               //Else we dont will be able to define which board was  
        #error Plataform not defined 

    #endif 

    /// IMPORTANT NAMES IN A PROTOCOL COMUNICATION 
    /// FCS -> FRAME CHECK SEQUENCY 
    /// OOK -> ON OFF KEYING
    /// ASK -> AMPLITUDE SHIFTH KEYING
    /// 


    /// SETING THE MACRO DEFINITIONS BELOW
    
    /// Maximum number of bytes to send + the FCS (Check sum) 
    #define MAX_MESSAGE_LEN 30

    /// The payload length => message = payload + FCS  
    /// PAYLOAD = "Carga Util"
    #define MAX_PAYLOAD MAX_MESSAGE_LEN-3

    /// Number of bits per character 
    #define RX_SAMPLES_PER_BIT 8 

    /// Define the speed defaut 
    #define MAX_SPEED_BPS 2000

    // Needed be read about this one !!!!!!--------------------------------------------------
    
    #define RX_RAMP_LEN 160

    // Ramp adjustment parameters 
    // Literally copying
    
    /// Internal ramp adjustment parameter
    #define RAMP_INC (RX_RAMP_LEN/RX_SAMPLES_PER_BIT)

    /// Internal ramp adjustment parameter
    #define RAMP_TRANSITION RX_RAMP_LEN/2

    /// Internal ramp adjustment parameter
    #define RAMP_ADJUST 9

    /// Internal ramp adjustment parameter
    #define RAMP_INC_RETARD (RAMP_INC-RAMP_ADJUST)

    /// Internal ramp adjustment parameter
    #define RAMP_INC_ADVANCE (RAMP_INC+RAMP_ADJUST)
    
    //-----------------------------------------------------------------------------------------

    /// 36 alternating 1/0 bits +
    /// 12 bits start bits +
    /// 4-6 bits encoded by count (message) +
    /// 2 bytes (16 bits) of FCS  
    #define HEADER_LEN 8 

    /// Pin used to transmit data
    void set_tx_pin(uint8_t pin);
    
    /// Pin used to receiving data
    void set_rx_pin(uint8_t pin);
    
    /// ptt (press to talk) used as a flag to made the transmitter enable
    void set_ptt_pin(uint8_t pin);
    
    /// The ptt pin goes to HIGH when the transmitter is enable
    /// If the ptt_invert is true, it's make goes to LOW
    void set_ptt_inverted(uint8_t inverted);

    /// Beging the communication with the communication's speed (bps)
    void setup_communication(uint16_t speed);

    /// Must be called before any message be received
    /// Inicialize the Phase Locked Loop (PLL)
    void rx_start();

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


    // If I decide to do an example, put the adress here 
    // (exclude the '//' ) /// @example name_of_example_code.pde  
    // (exclude the '//' ) /// A litlle explanation of what the code does

#endif