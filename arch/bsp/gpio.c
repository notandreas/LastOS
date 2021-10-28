#include <arch/bsp/gpio.h>

#define GPIO_BASE (0x7E200000 - 0x3F000000)
#define GPF_BITS 3

typedef struct _gpio_registers {
    unsigned int func[6];
    unsigned int reserved0;
    unsigned int set[2];
    unsigned int reserved1;
    unsigned int clr[2];
    unsigned int reserved2;
    unsigned int lev[2];
    unsigned int reserved3;
    unsigned int eds[2];
    unsigned int reserved4;
    unsigned int ren[2];
    unsigned int reserved5;
    unsigned int fan[2];
    unsigned int reserved6;
    unsigned int hen[2];
    unsigned int reserved7;
    unsigned int len[2];
    unsigned int reserved8;
    unsigned int aren[2];
    unsigned int reserved9;
    unsigned int afen[2];
    unsigned int reserved10;
    unsigned int pud;
    unsigned int pudclk[2];
    unsigned int reserved11;
    unsigned int reserved12;
} gpio;

static volatile gpio * const gpio_registers = (gpio *) GPIO_BASE;

void gpio_set_pin_func(int pin, gpio_func func) {
    switch (pin) {
        case 0 ... 9:
            gpio_registers->func[0] |= func << GPF_BITS * (pin % 10);
            break;
        case 10 ... 19:
            gpio_registers->func[1] |= func << GPF_BITS * (pin % 10);
            break;
        case 20 ... 29:
            gpio_registers->func[2] |= func << GPF_BITS * (pin % 10);
            break;
        case 30 ... 39:
            gpio_registers->func[3] |= func << GPF_BITS * (pin % 10);
            break;
        case 40 ... 49:
            gpio_registers->func[4] |= func << GPF_BITS * (pin % 10);
            break;
        case 50 ... 53:
            gpio_registers->func[5] |= func << GPF_BITS * (pin % 10);
            break;
        default:
            break;
    }
}