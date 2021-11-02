#ifndef UART_H
#define UART_H

/**
 * Setup UART
 * Pin 14 and 15 are set to ALT0 and pulldown.
 * Interrupt clear and setup.
*/
void init_uart();

void uart_enable();

void uart_disable();

void uart_put_c(char character);

#endif
