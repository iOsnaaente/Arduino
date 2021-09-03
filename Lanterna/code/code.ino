#include <avr/sleep.h>    // Para usar as funções de sleep do sistema 

#define ADC_PRESCALER_16  // Define o fator de conversão do clock (CLK/32) 

#define  POF PB4          // Pino 4 - Estado ligado ou desligado
#define  AD  PB3          // Pino 3 - ADC     
#define  BP  PB2          // Pino 2 
#define  BM  PB1          // Pino 1 - Has PWM
#define  LED PB0          // Pino 0 - Has interrupt 

#define POT_MAX 255       // Potência Máxima  
#define POT_MIN 0         // Potência Mínima

#define T_ON    4.5       // Tensão admissível para estar em 100%
#define T_HALF  4.0       // Tensão admissível para estar em 50%
#define T_OP    3.0       // Tensão admissível para estar ligado 


// Criação dos estados condicionais do código 
#define ON     1
#define HALF   2
#define DOWN   3
#define SLEEP  4

// EEPROM 
#define WRITE 1
#define READ  0

// mode == 0 ? Vcc as reference : Internal 1.1V as reference
#define ANALOG_REFERENCE( mode ) mode == 0 ? ADMUX &= ~(1 << REFS0) :  ADMUX |=  (1 << REFS0);  

// Potência de saída
static uint8_t POT_LAN;
static uint8_t STATE = ON; 

void sleep();

void setup(){
  
    #if defined  (ADC_PRESCALER_2 ) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC)                           );   // ADC prescaler 2
    #elif defined(ADC_PRESCALER_4 ) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS1)              );   // ADC prescaler 4
    #elif defined(ADC_PRESCALER_8 ) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS1) | _BV(ADPS0) );   // ADC prescaler 8
    #elif defined(ADC_PRESCALER_16) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2)              );   // ADC prescaler 16
    #elif defined(ADC_PRESCALER_32) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS0) );   // ADC prescaler 32
    #elif defined(ADC_PRESCALER_64) 
      ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) );   // ADC prescaler 64
    #endif 
    
    DDRB   |= 0b00001001;         // Configuram os pinos como entrada ou saída  //DDRB = DDRB | (1<<PB2) | (1<<PB1) // Equivalente de cima
    GIMSK  |= _BV(PCIE);          // Permite que os Pinos mudem o estado de Interrupções
    PCMSK  |= _BV(BP) | _BV(BM);  // Define os botões de entrada como interrupções 
    sei();                        // Habilita as interrupções

    // ADMUX -> 00=ADC0, 01=ADC1, 10=ADC2, 11=ADC3 
    ADMUX  |= (1<<MUX1)|(1<<MUX0);// ADCH=(b7:b2=0 b1=9 b0=8) ADCL=( b7=7 .... b0=0)  caso 1 inverte ( ADCL, ADCH )
    ADCSRA |= _BV(ADEN);          // Habilita os ADCs 
    
    // COM0A1:0 -> Compare Output Mode    - Normal =  00 
    // WGM02:0  -> WaveForm Generate Mode - Normal = 000
    // CS02:0   -> Clock Select prescale  - CLK/64 = 011 
    // FOC0A:B  -> Force Output Compare   - Não usado 
    TCCR0A = 0b10110000; //[COM0A1, COM0A0, COM0B1, COM0B0, -, -, WGM01, WGM00]         
    TCCR0B = 0b00000011; //[FOC0A , FOC0B , -, -  , WGM02 , CS02, CS01 , CS00 ]          
    
    POT_LAN = EEPROM_RW(0x00, 0x00, READ );
}

//OCR0A = 255;
//OCR0B = 255;

uint8_t T_DIV; 

void loop(){
    while( ADCSRA & _BV(ADSC) ); 
    POT_LAN = (ADCH<<6) | (ADCL & 0x3F ); 
    
    switch (STATE){    
      case ON:
        PORTB |= (1<<POF);    // ACENDE O PINO 4 PARA O STEP-UP 
        T_DIV  = 255;         // MANDA A TENSÃO DE POT_LAN ( 0, 100 )% 
        if( POT_LAN < T_ON )  // SE A POTENCIA FOR MENOR QUE T_ON
          STATE = HALF;       // ESCOA PARA HALF NO MESMO CICLO
        else                  // SEM O BREAK
          break;
        
      case HALF:
          PORTB |= (1<<POF);  // ACENDE O PINO 4 PARA O STEP-UP 
          T_DIV  = 127;        // MANDA A TENSÃO DE POT_LAN*0.5 
          if( POT_LAN < T_HALF )
            STATE = DOWN; 
          else
            break;
  
      case DOWN:
          PORTB |= (1<<POF);  // ACENDE O PINO 4 PARA O STEP-UP 
          T_DIV = 70;         // MANDA A TENSÃO DE POT_LAN*0.25 
          // INICIA A CONTAGEM PARA DESLIGAR A LANTERNA SE NENHUM BOTÃO FOR PRESSIONADO
          
          if( POT_LAN < T_OP )
            STATE = SLEEP;
          else 
            break;
      
      case SLEEP:
          PORTB |= ~(1<<POF);              // MANDA PIN4 PARA LOW
          T_DIV = 0;                       // MANDA A TENSÃO DE POT_LAN PARA 0 
          EEPROM_RW(0x00, POT_LAN, WRITE); // SALVA O VALOR DE POT_LAN NA EEPROM 0x00->BH e 0x01->BL 
          sleep();                         // CHAMA O SLEEP 
          break; 
    }
    
    OCR0A = T_DIV;

}


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


ISR ( PCINT0_vect ) {
    if ( STATE == SLEEP )
        STATE = ON;
    else
        if ( (PORTB & (1<<BP)) && (PORTB & (1<<BM))) 
            STATE = SLEEP;   
        else if ( PORTB & (1<<BP) ) 
            POT_LAN = (POT_LAN+5) < POT_MAX ? POT_LAN + 5 : POT_MAX;
        else if ( PORTB & (1<<BM) ) 
            POT_LAN = (POT_LAN-5) > POT_MIN ? POT_LAN - 5 : POT_MIN; 
}


uint8_t EEPROM_RW( uint8_t ADDR, uint8_t data, bool RW ){
    while ( EECR & (1<<EEPE) ){
        if ( RW ){
            EECR  = ( 0<<EEPM1 ) | ( 0>>EEPM0 ); // Seta o modo de programação -> 0 para Escrita 
            EEARL = ADDR;                        // Seta o endereço de escrita  
            EEDR  = data;                        // Escreve o byte no local addr 
            EECR |= (1<<EEMPE);                  // Seta EEMPE para finalizar
            EECR |= (1<<EEPE);                   // Inicia a escrita setando EEPE
            return 0; 
        }else{            
              EEARL = ADDR;                        // Seta o endereço de leitura 
              EECR |= (1<<EERE);                   // Seta EERE para começar a ler
              return EEDR;                         // Retorna o byte lido em EEDR 
        }
    }
}