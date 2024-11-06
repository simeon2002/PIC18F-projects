#include "CONFIG.h"

#define _XTAL_FREQ 64000000

void main(void) {
        
     // Ensure proper pin configuration
    ANSELF &= ~(1<<0);  // Disable analog function for RF3
    TRISB &= ~(1<<0);   // Set RF3 as output
//    WPUF &= ~(1<<0);    // Disable weak pull-up
    TRISB4 = 1;
    ANSELB4 = 0;
    WPUB4 = 1;
    TRISF3 = 0;
    
    
    while(1) {
        if (PORTB == 0X10) LATB0 = 1;
        else LATB0 = 0;
        
//        LATF3 = 1;
//        __delay_ms(500);
//        LATF3 = 0;
//        __delay_ms(500);
    }
    
    return;
    
    return;
}
