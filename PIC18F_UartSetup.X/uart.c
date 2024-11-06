/*
 * File:   uart.c
 * Author: simeo
 *
 * Created on November 2, 2024, 2:17 AM
 * 
 * This file is used for a setup on PIC18F57Q43 with interrupts. It is IMPORTANT
 * TO NOTE that the current version is just a normal uart tranmission and receiver
 * unit. No parity bit control or address transmission, neither flow control 
 * (HW or SW wise) for RX has been set up.
 */

// NOTE: pins to be used need to be configured separately!
/*
 * TODO:
 * - set up error handling mechanisms (receive FIFO overflow, framing error)
 * - use error handling mechanisms such checksum or collision detection (only for uart 1 on pic18
 
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RX_BUFFER_SIZE 64
#define UART_MODE 0
#define BAUD_RATE 0x1A0

char *TX_buffer = NULL; // buffer in order to store TX string.
int TX_position = 0;
int TX_length = 0;

// circular RX array-based buffer
char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_tail = 0; // tail = where data goes out
uint8_t rx_buffer_head = 0; // head = where data comes in 

void UART_init() {
    PIE4bits.U1IE = 1;

    // TX init
    U1CON0bits.MODE = UART_MODE;
    U1CON0bits.BRGS = 0; // 16 clocks / symbol
    U1CON0bits.TXEN = 0;
    U1BRG = BAUD_RATE;
    U1CON2bits.TXPOL = 0;
    // PIE4bits.U1TXIE = 1; enable when you want to start sending data to TX!
    
    // use TXEN to enable the UART transmitter circuitry.
    
    
    // RX init
    U1CON0bits.RXEN = 1; // enable RX
    PIE4bits.U1RXIE = 1; // setup RX receive interrupt  
    
    // uart module on
    U1CON1bits.ON = 1;
}

void __interrupt(irq(U1TX)) UART_TX_ISR(void) {
    
    // store ch into buffer
    if (TX_buffer && TX_position < TX_length) {
        U1TXB = TX_buffer[TX_position++];
    } else {
        PIE4bits.U1TXIE = 0; 
        TX_buffer = NULL;
        TX_length = 0;
        TX_position = 0;
        U1CON0bits.TXEN = 0; // disable UART_TX as Transmission finished
    }
}

void __interrupt(irq(U1RX)) UART_RX_ISR(void) {
    // todo: add flow control when RX is currently full => reason: no data loss.
    
    uint8_t received_byte = U1RXB;
    rx_buffer[rx_buffer_head] = received_byte;
    rx_buffer_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
}

void UART_send_word(char* str) {
    U1CON0bits.TXEN = 1;
    if (!str) return; // no string has been provided!
    while(PIE4bits.U1TXIE);  // Don't overwrite ongoing transmission

    TX_buffer = str;
    TX_length = (int)strlen(TX_buffer);
    TX_position = 0;
    PIE4bits.U1TXIE = 1; // enable interrupt.
}

void UART_receive_byte_of_length(char* buffer, uint8_t length) { 
    for (int i = 0; i < length; i++) {
        buffer[i] = rx_buffer[rx_buffer_tail];
        rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
    }
}