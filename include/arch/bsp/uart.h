#ifndef UART_H
#define UART_H

#include <stdint.h>

#include <arch/bsp/gpio.h>
#include <config.h>

/**
 * Setup UART
 * Pin 14 and 15 are set to ALT0 and pulldown.
 * Interrupt clear and setup.
*/
void init_uart();

/**
 * Enable UART and only TXE and RXE;
*/
void uart_enable();

/**
 * Disable UART.
*/
void uart_disable();

/**
 * Send a Char to the Terminal.
*/
void uart_put_c(char character);

/**
 * Get a Char from the Terminal.
*/
char uart_get_c();

void uart_add_to_buffer(char c);

char uart_next();

int uart_has_next();

#endif
