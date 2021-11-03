#include <stdint.h>

#include <arch/bsp/uart.h>
#include <arch/bsp/gpio.h>

#define UART_BASE (0x7E201000 - 0x3F000000)

//flag reg
enum uart_fr {
	RXFE = 0x4, 	// Receive FIFO empty
	TXFF = 0x5, 	// Transmit FIFO full
};

enum uart_lcrh {
	FEN = 0x4,     	// Enable FIFOs
	WLEN = 0x5, 	// Word length
};

enum uart_wlen {    //word len of wlen register
    _8bit = 0x11    // 8 bits
    _7bit = 0x10    // 7 bits
    _6bit = 0x01    // 6 bits
    _5bit = 0x00    // 5 bits
};

enum uart_cr {
	TXE = 0x8, 	   // Transmit enable
	RXE = 0x9, 	   // Receive enable
};

enum uart_imsc {
	CTSMIM = 0x1,  // nUARTCTS modem interrupt mask
	RXIM = 0x4,    // Receive interrupt mask
    TXIM = 0x5,    // Transmit interrupt mask
	RTIM = 0x6,    // Receive timeout interrupt mask
    FEIM = 0x7,    // Framing error interrupt mask
	PEIM = 0x8,    // Parity error interrupt mask
    BEIM = 0x9,    // Break error interrupt mask
	OEIM = 0x10,   // Overrun error interrupt mask
};

//interrupt control register
enum uart_icr {
	CTSMIC = 0x1, 	// nUARTCTS modem masked interrupt status
	RXIC = 0x4, 	// Receive masked interrupt status
    TXIC = 0x5, 	// Transmit interrupt clear
	RTIC = 0x6, 	// Receive timeout interrupt clear.
    FEIC = 0x7, 	// Framing error interrupt clear
	PEIC = 0x8, 	// Parity error interrupt clear
    BEIC = 0x9, 	// Break error interrupt clear
	OEIC = 0x10,    // Overrun error interrupt clear
};


typedef struct _uart_registers {
    uint32_t dr;        //datareg
    uint32_t rsrecr;    //receive status register/error clear register
    uint32_t reserved[4];
    uint32_t fr;        //flag register
    uint32_t ilpr;      //dont care
    uint32_t ibrd;      //integer part of the baud rate divisor value register
    uint32_t fbrd;      //fractional part of the baud rate divisor value register
    uint32_t lcrh;      //line control register
    uint32_t cr;        //control register
    uint32_t ifls;      //interrupt FIFO level select register
    uint32_t imsc;      //interrupt mask set/clear register
    uint32_t ris;       //raw interrupt status register
    uint32_t mis;       //masked interrupt status register
    uint32_t icr;       //interrupt clear register
    uint32_t dmacr;     //dont care
    uint32_t itcr;      //test control register
    uint32_t itip;      //test control register
    uint32_t itop;      //test control register
    uint32_t tdr;       //test data register
} uart;

static volatile uart * const uart_registers = (uart *) UART_BASE;

void init_uart() {
    uart_registers->cr = 0;

    uart_registers->icr = (1 << CTSMIC) | (1 << RXIC) | (1 << TXIC) | (1 << RTIC) | (1 << FEIC) | (1 << PEIC) | (1 << BEIC) | (1 << OEIC);
    uart_registers->lcrh &= ~(1 << FEN);

    gpio_set_pin_func(14, GPIO_FUNC_ALT0);      //ALT0 Func for GPIO pins 14 and 15 (UART transmit and receive)
    gpio_set_pin_func(15, GPIO_FUNC_ALT0);

    gpio_set_up_down(14, GPIO_UD_PULL_DOWN);    //setting the GPIO pins 14 and 15 to Pull_Down
    gpio_set_up_down(15, GPIO_UD_PULL_DOWN);

    uart_registers->imsc = (1 << CTSMIM) | (1 << RXIM) | (1 << TXIM) | (1 << RTIM) | (1 << FEIM) | (1 << PEIM) | (1 << BEIM) | (1 << OEIM);
    uart_registers->lcrh |= (1 << FEN) | (_8bit << WLEN);  // enabling FIFOs, setting word len to 8 bits
    uart_registers->cr = 1 | (1 << TXE) | (1 << RXE);  //enable uart, transmit and receive
}

void uart_enable() {
    uart_registers->cr = 1 | (1 << TXE) | (1 << RXE);  //enable uart, transmit and receive
}

void uart_disable() {
    uart_registers->cr = 0;                            //disabling uart
}

void uart_put_c(char character) {
    while ((uart_registers->fr) & (1 << TXFF)) {}
    uart_registers->dr = (character);
}

char uart_get_c() {
    while ((uart_registers->fr) & (1 << RXFE)) {}
    return uart_registers->dr;
}
