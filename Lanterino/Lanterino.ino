#include <avr/interrupt.h> // Para usar as interrupções  
#include <avr/sleep.h>     // Para usar as funções de sleep do sistema  
#include <avr/io.h>

// Set Bit In and Clear Bit In
#define sbi(reg, bit) (_SFR_BYTE(reg) |=  _BV(bit) )
#define cbi(reg, bit) (_SFR_BYTE(reg) &= ~_BV(bit) )

// Button get pressed - Return 1 if was 0 if not
#define bgp( button ) (!(PINB & (1 << button)))

#define INPUT        ( 0 )
#define OUTPUT       ( 1 )

#define LOW          ( 0 )
#define HIGH         ( 1 )

// Mapeamento dos pinos do attiny
#define  POF PB3          // Pino 3 - Estado ligado ou desligado 
#define  AD  A2           // era PB4, analog read nao aceita, tem que ser ((((((((((A2)))))))))      Pino 3 - Leitor ADC da bateria    
#define  BP  PCINT2       // Pino 2 - Botão + 
#define  BM  PCINT1       // Pino 1 - Botão - 
#define  LED PB0          // Pino 0 - LED com saída PWM  

#define POT_MAX (252)     // Potência Máxima  - Divisível por 2 e 3
#define POT_MIN ( 0 )     // Potência Mínima


#define T_ON    (770)     //3.6       // Tensão admissível para estar em 100%   - 877=4.01
#define T_HALF  (710)     //3.3       // Tensão admissível para estar em 50%
#define T_OP    (650)     //3.0       // Tensão admissível para estar ligado 


// Criação dos estados condicionais do código
#define ON     1
#define HALF   2
#define DOWN   3
#define SLEEP  4


// DECLARAÇÃO DAS FUNÇÕES USADAS NO CÓDIGO
uint8_t EEPROM_READ( uint8_t ADDR );
void    EEPROM_WRITE( uint8_t ADDR, uint8_t data );
void    pin_mode(uint8_t pin, uint8_t mode);
void    sleep();


// VARIÁVEIS GLOBAIS DO SISTEMA
static uint8_t  T_BAT = 1023;
static uint8_t  T_LAN =   0;
static uint8_t  STATE =  ON;
static uint8_t  DUTY  = 255;
static uint16_t COUNT =   0;


void setup() {
  cli();

  // DEFINIÇÃO DOS LEDS DE SAÍDA
  pin_mode( POF, OUTPUT );
  pin_mode( LED, OUTPUT );

  // CONTROLA OS BOTÕES E CONFIGURA SUAS CONDIÇÕES
  pin_mode( BP, INPUT );        //DDX 0 PORTx 1 PUD 0 = Input pull-up
  pin_mode( BM, INPUT );

  cbi( MCUCR, PUD );            // PUD = PULL UP DISABLE = desabilitado
  sbi( PORTB, BM  );            // Seta o valor da PORTB para ter um PULLUP forçado
  sbi( PORTB, BP  );            // Seta o valor da PORTB para ter um PULLUP forçado

  GIMSK  |= _BV(PCIE);          // Permite que os Pinos mudem o estado de Interrupções
  PCMSK  |= _BV(BP) | _BV(BM);  // Define os botões de entrada como interrupções

  sbi(MCUCR, ISC01 );           // MCUCR - MCU(microcontroler unit) Control Register ISC01 = FALLING

  // CONFIGURAÇÃO DO ADC USADO NO MEDIDOR DE TENSÃO ( ADC3 )
  // ADMUX -> 00=ADC0, 01=ADC1, 10=ADC2, 11=ADC3
  // ADCH=(b7:b2=0 b1=9 b0=8) ADCL=( b7=7 .... b0=0)  caso 1 inverte ( ADCL, ADCH )
  ADCSRA  = _BV( ADEN );            //Habilita os ADCs
  ADMUX   = _BV(MUX1) | _BV(REFS0); // set PB4 as analog input and use internal voltage reference  <<<<<<<<<<<<<<<<<<< habilita o ADC, sem isso le 0volts.
  ADCSRA |= _BV( ADIE );

  // CONFIGURAÇÃO DO TIMER E DOS PINOS DE INTERRUPÇÃO
  // COM0A1:0 -> Compare Output Mode    - Normal      =  00
  // WGM02:0  -> WaveForm Generate Mode - CTC counter = 010 FastPWM = 011
  // TCCR0A = 0b10110011; //[COM0A1, COM0A0, COM0B1, COM0B0, -, -, WGM01, WGM00]
  // TCCR0A |= _BV(COM0A1) | _BV(COM0B1) | _BV(COM0B0) | _BV(WGM01) | _BV(WGM00);
  DDRB   |= _BV(PB0);                   // set PWM pin as OUTPUT
  TCCR0A |= _BV(WGM01) | _BV(WGM00);    // set timer mode to FAST PWM
  TCCR0A |= _BV(COM0A1);                // connect PWM signal to pin (AC0A => PB0)

  // CONFIGURAÇÕES DO TIMER
  // FOC0A:B  -> Force Output Compare   - Não usado
  // CS02:0   -> Clock Select prescale  - CLK/1204/256 = 4.6Hz = 101
  // TCCR0B = 0b00000101; //[FOC0A , FOC0B , -, -  , WGM02 , CS02, CS01 , CS00 ]
  TCCR0B  = _BV(CS01);

  OCR0B   = 0x00;                   // Comparador de saída
  TCNT0   = 0;                      // Seta o contador em 0
  TIMSK0 |= _BV(TOIE0);             // Habilita o Timer Overflow Interrupt

  DUTY = EEPROM_READ( 0x00 );
  sei();
}

void loop() {
  //while {1}//( ADCSRA & _BV(ADSC) );
  //(ADCSRA |= _BV(ADSC));
  //T_BAT = (ADCH << 6) | (ADCL & 0x3F );
  //T_BAT = 4;

  delayyy(150);
  T_BAT = analogRead( A2 );

  if ( bgp(BP) ) {
    T_LAN = ((T_LAN + 6) < POT_MAX) ? (T_LAN + 6) : POT_MAX;
    if ( T_LAN == POT_MAX )
      blinkyyy();
    COUNT = 0;
  }

  if ( bgp(BM) ) {
    T_LAN = ((T_LAN - 6) > POT_MIN) ? (T_LAN - 6) : POT_MIN;
    if ( T_LAN == POT_MIN )
      blinkyyy();
    COUNT = 0;
  }

  switch ( STATE ) {
    case ON:
      DUTY   = T_LAN ;
      if ( T_BAT < T_ON )       STATE = HALF;
      else                      break;

    case HALF:
      DUTY =  T_LAN / 2;
      if ( T_BAT < T_HALF )       STATE = DOWN;
      else if ( T_BAT > T_ON )    STATE = ON;
      else                        break;

    case DOWN:
      DUTY   = T_LAN / 3;
      if ( T_BAT < T_OP )         STATE = SLEEP;
      else if ( T_BAT > T_HALF )  STATE = HALF;
      else                        break;

    case SLEEP:
      cbi(PORTB, POF);
      EEPROM_WRITE( 0x00, DUTY );
      DUTY   = 0;
      anawrite(DUTY);
      delayyy(1250);
      sleep();
  }

  anawrite(DUTY);
  sbi(PORTB, POF);
  delayyy(150);
}


// FUNÇÃO DE INTERRUPÇÃO PARA CONTROLE DO BRILHO E (ON/OFF)
ISR ( PCINT0_vect ) {
  if ( STATE != SLEEP) {
    if ( bgp(BP) && bgp(BM) ) {
      STATE = SLEEP;
    }
  } else {
    STATE = ON;
  }
}


ISR ( TIM0_OVF_vect ) {
  if ( STATE != SLEEP ) {
    COUNT ++;
    if (COUNT == 65000 ) { //2400=10s  72000=300s 5min
      blinkyyy();
      STATE = SLEEP;
      COUNT = 0;
    }
  } else {
    COUNT = 0;
  }
}

/*
ISR(ADC_vect) {
  volatile uint16_t samples_sum = 0;
  volatile uint8_t  samples_cnt = 0;
  uint8_t high, low;

  if (samples_cnt < 32) {
    low  = ADCL;
    high = ADCH;
    samples_sum += (high << 8) | low;
    samples_cnt++;
    ADCSRA |= _BV(ADSC);
  }
}
*/


void sleep() {
  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep
  cli();                                  // Disable interrupts
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on
  sei();                                  // Enable interrupts
}

// ESCREVE NO ENDEREÇO ADDR DA EEPROM
void EEPROM_WRITE( uint8_t ADDR, uint8_t data ) {
  while ( EECR & (1 << EEPE) ) {
    EECR  = ( 0 << EEPM1 ) | ( 0 >> EEPM0 ); // Seta o modo de programação -> 0 para Escrita
    EEARL = ADDR;                            // Seta o endereço de escrita
    EEDR  = data;                            // Escreve o byte no local addr
    EECR |= (1 << EEMPE);                    // Seta EEMPE para finalizar
    EECR |= (1 << EEPE);                     // Inicia a escrita setando EEPE
  }
}

// LÊ NO ENDEREÇO ADDR DA EEPROM
uint8_t EEPROM_READ( uint8_t ADDR ) {
  while ( EECR & (1 << EEPE) ) {
    EEARL = ADDR;                            // Seta o endereço de leitura
    EECR |= (1 << EERE);                     // Seta EERE para começar a ler
    return EEDR;                             // Retorna o byte lido em EEDR
  }
}

// SETA O PINO COMO ENTRADA / SAÍDA OU SAÍDA COM PULLUP
void pin_mode(uint8_t pin, uint8_t mode) {
  uint8_t sreg;
  if (mode == OUTPUT) {
    sreg = SREG;
    cli();
    sbi(DDRB, pin);
    SREG = sreg;

  } else if ( mode == INPUT) {
    sreg = SREG;
    cli();
    cbi(DDRB, pin);
    SREG = sreg;

  } else if ( mode == INPUT_PULLUP ) {
    sbi( PORTB, pin );
    pin_mode( pin, INPUT );
  }
}


void anawrite(uint8_t dutty) {
  //analogWrite(LED, dutty);
  OCR0A = dutty;
}


void delayyy(int delayy) {
  _delay_ms(delayy);
}


void blinkyyy() {
  for (int n = 0; n < 25; n++) {
    if ( bgp(BP) || bgp(BM) )  {
      //bpressed = 1;
      //   exit(1);
      break;
    }
    DDRB &= ~(1 << PB0); // desabilita pwm

    delayyy(500);

    DDRB |= _BV(PB0); // habilita pwm

    anawrite(DUTY);

    if ( bgp(BP) || bgp(BM) )  {
      //bpressed = 1;
      //   exit(1);
      break;
    }
    delayyy(250);
  }
}
