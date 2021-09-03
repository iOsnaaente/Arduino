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

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit) )
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit) )

#define bgp( button ) (!(PINB & (1 << button)))

uint8_t digital_read(uint8_t pin) {
  if ((PINB & (1 << pin)) > 0)
    return HIGH;
  return LOW;
}

void digital_write(uint8_t pin, uint8_t value) {
  uint8_t sreg;
  if (value == HIGH) {
    sreg = SREG;
    cli();
    sbi(PORTB, pin);
    SREG = sreg;
  } else { // value == LOW
    sreg = SREG;
    cli();
    cbi(PORTB, pin);
    SREG = sreg;
  }
}

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
  pin_mode( LED, OUTPUT );
  pin_mode( POF, OUTPUT );
  pin_mode( BP , INPUT  );
  pin_mode( BM , INPUT  );

  cbi(MCUCR, PUD ); // PUD = PULL UP DISABLE = desabilitado

  sbi( PORTB, BM );
  sbi( PORTB, BP );

  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT1) | _BV(PCINT2);
  sei();
}

static int DUTY = 255;

void loop() {

  if ( bgp(BP) && bgp(BM) ) {
    DUTY = 0;
  } else {
    if ( bgp(BP) ) {
      DUTY = (DUTY + 10) < 252 ? DUTY + 10 : 252;
    }
    if ( bgp(BM) ) {
      DUTY = (DUTY - 10) > 60   ? DUTY - 10 :   60;
    }
    analogWrite( LED, DUTY );
  }
  delay(100);
}


ISR ( PCINT0_vect ) {
}

// SETA O PINO COMO HIGH
void pinHigh(uint8_t pin) {
  uint8_t sreg = SREG;
  cli();
  sbi(PORTB, pin);
  SREG = sreg;
}

// SETA O PINO COMO LOW
void pinLow( uint8_t pin ) {
  uint8_t sreg = SREG;
  cli();
  cbi(PORTB, pin);
  SREG = sreg;
}


// INVERTE OS VALORES DO PINO
void pinToggle( uint8_t pin ) {
  if ( PORTB & (1 << pin) > 0)
    pinLow(pin);
  else
    pinHigh(pin);
}
