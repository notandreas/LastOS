#include <arch/bsp/uart.h>

#define UART_BASE (0x7E201000 - 0x3F000000)

/**
 * @brief The Ringbuffer of the uart input.
 * 
 */
char uart_buffer[UART_INPUT_BUFFER_SIZE] = {0};
int uart_buffer_ptr = 0;                            // ptr to the next free element
int uart_queue_ptr = 0;                             // ptr to the next element in buffer
int full_buffer = 0;                                // if the buffer is full or empty


/**
 * @brief UART FR register flags
 * 
 */
enum uart_fr {
    RXFE = 1 << 0x4, 	                            // Receive FIFO empty
    TXFF = 1 << 0x5, 	                            // Transmit FIFO full
};


/**
 * @brief UART LCRH register flags
 * 
 */
enum uart_lcrh {
    FEN = 1 << 0x4,    	                            // Enable FIFOs
    WLEN = 0b11<< 0x5, 	                            // Word length
};


/**
 * @brief UART CR register flags (controle register)
 * 
 */
enum uart_cr {
    DISABLE_UART = 0x0,                             // Disable UART
    ENABLE_UART = 0x1,                              // Enable UART
    TXE = 1<< 0x8, 	                                // Transmit enable
    RXE = 1<< 0x9, 	                                // Receive enable
};


/**
 * @brief UART IMSC register flags (interrupts)
 * 
 */
enum uart_imsc {
    CTSMIM = 1 << 0x1,                              // nUARTCTS modem interrupt mask
    RXIM = 1 << 0x4,                                // Receive interrupt mask
    TXIM = 1 << 0x5,                                // Transmit interrupt mask
    RTIM = 1 << 0x6,                                // Receive timeout interrupt mask
    FEIM = 1 << 0x7,                                // Framing error interrupt mask
    PEIM = 1 << 0x8,                                // Parity error interrupt mask
    BEIM = 1 << 0x9,                                // Break error interrupt mask
    OEIM = 1 << 0x10,                               // Overrun error interrupt mask
};


/**
 * @brief UART ICR register flags
 * 
 */
enum uart_icr {
    CTSMIC = 1 << 0x1, 	                            // nUARTCTS modem masked interrupt status
    RXIC = 1 << 0x4, 	                            // Receive masked interrupt status
    TXIC = 1 << 0x5, 	                            // Transmit interrupt clear
    RTIC = 1 << 0x6, 	                            // Receive timeout interrupt clear.
    FEIC = 1 << 0x7, 	                            // Framing error interrupt clear
    PEIC = 1 << 0x8, 	                            // Parity error interrupt clear
    BEIC = 1 << 0x9, 	                            // Break error interrupt clear
    OEIC = 1 << 0x10,                               // Overrun error interrupt clear
};


/**
 * @brief UART Peripherie struct that contanes all register 
 * 
 */
typedef struct _uart_registers {
    uint32_t dr;                                    // datareg 
    uint32_t rsrecr;                                // receive status register/error clear register 0x4
    uint32_t reserved[4];                           // 0x8 0x0c 0x10 0x14
    uint32_t fr;                                    // flag register 0x18
    uint32_t reserved1;                             // 0x1c
    uint32_t ilpr;                                  // dont care
    uint32_t ibrd;                                  // integer part of the baud rate divisor value register 0x20
    uint32_t fbrd;                                  // fractional part of the baud rate divisor value register
    uint32_t lcrh;                                  // line control register
    uint32_t cr;                                    // control register
    uint32_t ifls;                                  // interrupt FIFO level select register
    uint32_t imsc;                                  // interrupt mask set/clear register
    uint32_t ris;                                   // raw interrupt status register
    uint32_t mis;                                   // masked interrupt status register
    uint32_t icr;                                   // interrupt clear register
    uint32_t dmacr;                                 // dont care
    uint32_t itcr;                                  // test control register
    uint32_t itip;                                  // test control register
    uint32_t itop;                                  // test control register
    uint32_t tdr;                                   // test data register
} uart;

static volatile uart * const uart_registers = (uart *) UART_BASE;


void init_uart() {
    uart_registers->cr = DISABLE_UART;                                                  // disable uart

    uart_registers->icr = CTSMIC | RXIC | TXIC | RTIC | FEIC | PEIC |  BEIC | OEIC;     // clear interrupts
    uart_registers->lcrh &= ~FEN;                                                       // flush FIFO

    gpio_set_pin_func(14, GPIO_FUNC_ALT0);                                              // ALT0 Func for GPIO pins 14 and 15 (UART transmit and receive)
    gpio_set_pin_func(15, GPIO_FUNC_ALT0);

    gpio_set_up_down(14, GPIO_UD_PULL_DOWN);                                            // setting the GPIO pins 14 and 15 to Pull_Down
    gpio_set_up_down(15, GPIO_UD_PULL_DOWN);

    uart_registers->imsc |= RXIM;
    uart_registers->lcrh |= WLEN;                                                       // disable FIFO, setting word len to 8 bits
    uart_registers->cr = 1 | TXE | RXE;                                                 // enable uart, transmit and receive
}


void uart_enable() {
    uart_registers->cr = ENABLE_UART | TXE | RXE;                                       // enable uart, transmit and receive
}


void uart_disable() {
    uart_registers->cr = DISABLE_UART;                                                  // disabling uart
}


void uart_put_c(char c) {
    while ((uart_registers->fr) & TXFF) {}
    uart_registers->dr = (c);
}


char uart_get_c() {
    while ((uart_registers->fr) & RXFE) {}
    return uart_registers->dr;
}


void uart_add_to_buffer(char c) {
    if (full_buffer)
        panic("UART Buffer is full!");

    if ((uart_buffer_ptr + 1) % UART_INPUT_BUFFER_SIZE == uart_queue_ptr)
        full_buffer = 1;

    uart_buffer[uart_buffer_ptr++] = c;
    uart_buffer_ptr %= UART_INPUT_BUFFER_SIZE;
}


char uart_next() {
    if (uart_queue_ptr == uart_buffer_ptr && !full_buffer)
        panic("UART Buffer is empty!");

    full_buffer = 0;

    char c = uart_buffer[uart_queue_ptr++];
    uart_queue_ptr %= UART_INPUT_BUFFER_SIZE;

    return c;
}


int uart_has_next() {
    return !(uart_queue_ptr == uart_buffer_ptr && !full_buffer);
}
