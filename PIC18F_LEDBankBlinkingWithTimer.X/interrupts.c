#include "interrupts.h"
#include "stdbool.h"    

bool LED_on_flag = false;
uint8_t LED_counter = 1;

void INTERRUPT_initialize() {
    INTCON0bits.GIE = 1; // enable global interrupts
    INTCON0bits.IPEN = 0; // disable priorities
    // IVTbase = 0x0008 by default
    
    // setup EXTI 1
    PIR6bits.INT1IF = 0;
    PIE6bits.INT1IE = 1;
    INT1PPS = 0x09;
    INTCON0bits.INT1EDG = 1;
    // note: when entering interrupt of f.e. button, switch interrupt off 
    // during execution AND reset IRflag. That way, no bouncing of switch occurs
    
    // setup timer IR
    PIR3bits.TMR0IF = 0;
    PIE3bits.TMR0IE = 1;    
}

void __interrupt(irq(INT1)) BUTTON_ISR(void) {
    PIE6bits.INT1IE = 0; // for debouncing
    PIR6bits.INT1IF = 0; // reset IRflag
    
    LED_on_flag = !LED_on_flag;
    
    if (LED_on_flag) {
        T0CON0bits.EN = 1;
        LED_on_flag = true;
    } else {
        T0CON0bits.EN = 0;
        PORTC = 0;
        LED_counter = 1;
    }
    
    PIE6bits. INT1IE = 1;
}

void __interrupt(irq(TMR0)) TMR0_ISR(void) {
    PIR3bits.TMR0IF = 0;
    
    if (LED_counter >= 0x80) LED_counter = 1;
    
    PORTC = LED_counter;
    LED_counter *= 2;
    
  
}