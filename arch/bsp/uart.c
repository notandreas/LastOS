#include <arch/bsp/uart.h>
#include <stdint.h>

#define UART_BASE (0x7E201000 - 0x3F000000)


typedef struct _uart_registers {
    uint32_t dr;        //datareg
    uint32_t rsrecr;    //receive status register/error clear register
    uint32_t queue[4];
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

