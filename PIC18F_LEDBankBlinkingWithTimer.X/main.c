/*
 * File:   newmain.c
 * Author: simeo
 *
 * Created on October 31, 2024, 4:01 PM
 */


#include "CONFIG.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000

void main(void) {
    // port C LED bank configuration
    TRISC = 0X00;
    WPUC = 0Xff;
    PORTC = 0x00;
    
    // PORT A: button config
    TRISBbits.TRISB1 = 0b1;
    ANSELBbits.ANSELB1 = 0b0;
    
    // timer setup 
    T0CON0bits.MD16 = 0;
    T0CON1bits.CS = 0b010;
    T0CON1bits.ASYNC = 0;
    T0CON1bits.CKPS = 0b1111;
    T0CON0bits.EN = 0;
    
    INTERRUPT_initialize();
    
    
    while(1) {};
    
    
}
