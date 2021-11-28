#ifndef UART_H
#define UART_H

#include <stdint.h>

#include <arch/bsp/gpio.h>
#include <config.h>

/**
 * @brief Setup UART:
 * Pin 14 and 15 are set to ALT0 and pulldown.
 * Interrupt clear and setup.
 * 
 */
void init_uart();


/**
 * @brief Enable UART and only TXE and RXE;
 * 
 */
void uart_enable();


/**
 * @brief Disable UART.
 * 
 */
void uart_disable();


/**
 * @brief Send a Char to the Terminal.
 * 
 * @param character 
 */
void uart_put_c(char c);


/**
 * @brief Get a Char from the Terminal.
 * 
 * @return The char.
 */
char uart_get_c();


/**
 * @brief Add char (c) to the ringbuffer
 * 
 * @param c The char.
 */
void uart_add_to_buffer(char c);


/**
 * @brief Get the FIFO element of the ringbuffer.
 * 
 * @return The char.
 */
char uart_next();


/**
 * @brief Check if the ringbuffer is not empty.
 * 
 * @return return 1 if has a char in ringbuffer.
 */
int uart_has_next();

#endif
