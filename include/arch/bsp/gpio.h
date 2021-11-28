#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#include <kernel/panic.h>

/**
 * @brief Defines all GPIO functions for the FSELn of the GPFSELm.
 * 
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


/**
 * @brief The (pin) mode.
 * 
 */
typedef enum _gpio_up_down {
    GPIO_UD_DISABLE = 0x00,
    GPIO_UD_PULL_DOWN = 0x01,
    GPIO_UD_PULL_UP = 0x10,
    GPIO_UD_RESERVED = 0x11,
} gpio_up_down;


/**
 * @brief Set the GPIO GPFSELn register for Pin (pin).
 * 
 * @param pin The Pin range goes from 0 to 53. If is not in range, this function panic.
 * @param func The function mode of the pin (gpio_func)
 */
void gpio_set_pin_func(int pin, gpio_func func);


/**
 * @brief Set a (pin) mode.
 * 
 * @param pin The Pin range goes from 0 to 53. If is not in range, this function does nothing.
 * @param func How the (pin) ist set (gpio_up_down).
 */
void gpio_set_up_down(int pin, gpio_up_down func);

#endif
