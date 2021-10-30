#include <arch/bsp/uart.h>
#include <arch/bsp/gpio.h>
#include <stdint.h>

#define UART_BASE (0x7E201000 - 0x3F000000)


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

    uart_registers->icr = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);
    uart_registers->lcrh &= ~(1 << 4);

    gpio_set_pin_func(14, GPIO_FUNC_ALT0);
    gpio_set_pin_func(15, GPIO_FUNC_ALT0);

    gpio_set_up_down(14, GPIO_UD_PULL_DOWN);
    gpio_set_up_down(15, GPIO_UD_PULL_DOWN);

    uart_registers->imsc = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 8) | (1 << 10);
    uart_registers->lcrh |= (1 << 4) | (0x11 << 5); 
    uart_registers->cr = 1 | (1 << 8) | (1 << 9);
}
