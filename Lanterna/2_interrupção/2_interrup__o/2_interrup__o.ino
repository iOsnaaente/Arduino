#include <avr/interrupt.h> // Para usar as interrupções  
#include <avr/sleep.h>     // Para usar as funções de sleep do sistema  
#include <avr/interrupt.h>
#include <avr/io.h>

#define LOW     ( 0 )
#define HIGH    ( 1 )
#define INPUT   ( 0 )
#define OUTPUT  ( 1 )

#define  BP  PB2          // Pino 2 
#define  BM  PB1          // Pino 1 - Has PWM

#define  LED PB0          // Pino 0 - Has interrupt
#define  POF PB4          // Pino 4 - Estado ligado ou desligado


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit) )
#endif

#ifndef sbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit) )
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit) )
#endif


static uint8_t    T_LAN =  0;

uint8_t DUTY;


void pin_mode(uint8_t pin, uint8_t mode) {
  uint8_t sreg;
  if (mode == OUTPUT) {
    sreg = SREG;
    cli();
    sbi(DDRB, pin);
    SREG = sreg;
  } else { // mode == INPUT
    sreg = SREG;
    cli();
    cbi(DDRB, pin);
    SREG = sreg;
  }
}


void setup() {

  DDRB   |= 0b00001001;         // Configuram os pinos como entrada ou saída  //DDRB = DDRB | (1<<PB2) | (1<<PB1) // Equivalente de cima
  PORTB  |= 0b00000110;         //


  GIMSK |= ( 1 << PCIE  );
  PCMSK |= ( 1 << PCINT2) | ( 1 << PCINT1);
  sei();

}


void loop() {
}


ISR ( PCINT0_vect ) {
  if ( !(PINB & (1 << BP)) ) {
    sbi(PORTB, PB0 );
  } else {
    cbi( PORTB, PB0 );
  }
}
