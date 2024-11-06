#include <xc.h>

/*
 * Initialization of interrupt controller.
 */
void INTERRUPT_initialize();

void __interrupt(irq(INT0PPS)) BUTTON_ISR(void);