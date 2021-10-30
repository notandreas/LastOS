#ifndef GPIO_H
#define GPIO_H

/**
 * Defines all GPIO functions for the FSELn of the GPFSELm.
*/
typedef enum _gpio_func {
    GPIO_FUNC_INPUT = 0x000,
    GPIO_FUNC_OUTPUT = 0x001,
    GPIO_FUNC_ALT0 = 0x100,
    GPIO_FUNC_ALT1 = 0x101,
    GPIO_FUNC_ALT2 = 0x110,
    GPIO_FUNC_ALT3 = 0x111,
    GPIO_FUNC_ALT4 = 0x011,
    GPIO_FUNC_ALT5 = 0x010,
} gpio_func;

typedef enum _gpio_up_down {
    GPIO_UD_DISABLE = 0x00,
    GPIO_UD_PULL_DOWN = 0x01,
    GPIO_UD_PULL_UP = 0x10,
    GPIO_UD_RESERVED = 0x11,
} gpio_up_down;

/**
 * Set the GPIO GPFSELn register for Pin (pin).
 * If (pin) is not existend, this function does nothing.
 * The Pin (pin) range goes from 0 to 53.
*/
void gpio_set_pin_func(int pin, gpio_func func);

/**
 * Enable pull-up/down for Pin (pin).
 * GPPUD and GPPUDCLKn are set automatically.
 * The Pin (pin) range goes from 0 to 53.
*/
void gpio_set_up_down(int pin, gpio_up_down func);

#endif
