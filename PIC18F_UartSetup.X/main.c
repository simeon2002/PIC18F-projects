/*
 * File:   main.c
 * Author: simeo
 *
 * Created on November 2, 2024, 12:36 PM
 */


#include <xc.h>
#include "uart.h"
#include "CONFIG.h"
#include <stdlib.h>

void main(void) {
    // UART TX PPS
    RF2PPS = 0x20;
    TRISFbits.TRISF2 = 0;
    ANSELFbits.ANSELF2 = 0;
    // UART RX PPS
    U1RXPPS = 0b101011;     
    TRISFbits.TRISF3 = 1;
    ANSELFbits.ANSELF3 = 0;
    
    UART_init();             // initialize UART
            
    // enable global IRs
    INTCON0bits.GIE = 1; // enable global interrupts
    INTCON0bits.IPEN = 0; // disable priorities
    
    while(1) {
        UART_send_word("testing it out\n");
        __delay_ms(100);  // Add delay between transmissions
        for (int i = 0; i < 10; i++) {
            __delay_ms(1000);
        }
        char buffer[10];
        UART_receive_byte_of_length(buffer, 5);
        UART_send_word(buffer);
    }
}
