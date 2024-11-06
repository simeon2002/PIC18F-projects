#include <xc.h>

#define BAUD_RATE 9600

void UART_init(void);

void UART_send_word(char*);

void UART_receive_byte_of_length(char*, uint8_t);

void __interrupt(irq(U1TX)) UART_TX_ISR(void);