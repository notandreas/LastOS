#include <arch/bsp/gpio.h>
#include <stdint.h>

#define GPIO_BASE (0x7E200000 - 0x3F000000)
#define GPF_BITS 3

typedef struct _gpio_registers {
    uint32_t func[6];       //GPIO Function Select Registers
    uint32_t reserved0;
    uint32_t set[2];        //GPIO Output Set Registers
    uint32_t reserved1;
    uint32_t clr[2];        //GPIO Output Clear Registers
    uint32_t reserved2;
    uint32_t lev[2];        //GPIO PIN Level Registers (High/Low)
    uint32_t reserved3;
    uint32_t eds[2];        //GPIO Event Detect Status Registers
    uint32_t reserved4;
    uint32_t ren[2];        //GPIO Rising Edge Detect Enable Registers
    uint32_t reserved5;
    uint32_t fen[2];        //GPIO Falling Edge Detect Enable Registers
    uint32_t reserved6;
    uint32_t hen[2];        //GPIO High Detect Enable Registers
    uint32_t reserved7;
    uint32_t len[2];        //GPIO Low Detect Enable Registers
    uint32_t reserved8;
    uint32_t aren[2];       //GPIO Asynchronous rising Edge Detect Enable Registers
    uint32_t reserved9;
    uint32_t afen[2];       //GPIO Asynchronous Falling Edge Detect Enable Registers
    uint32_t reserved10;
    uint32_t pud;           //GPIO Pull-up/down Register
    uint32_t pudclk[2];     //GPIO Pull-up/down Clock Registers
    uint32_t reserved11;
    uint32_t reserved12;
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

void gpio_set_up_down(int pin, gpio_up_down up_down) {
    gpio_registers->pud = up_down;
    if (0 <= pin && pin <= 31) {
        gpio_registers->pudclk[0] = 1 << pin;
    }
    else if (32 <= pin && pin <= 53) {
        gpio_registers->pudclk[1] = 1 << (pin % 32);
    }
}
