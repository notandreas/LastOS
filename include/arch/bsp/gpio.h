#ifndef GPIO_H
#define GPIO_H

typedef enum _gpio_func {
	GPIO_INPUT = 0x000,
	GPIO_OUTPUT = 0x001,
    GPIO_ALT0 = 0x100,
    GPIO_ALT1 = 0x101,
    GPIO_ALT2 = 0x110,
    GPIO_ALT3 = 0x111,
    GPIO_ALT4 = 0x011,
    GPIO_ALT5 = 0x010,
} gpio_func;

void gpio_set_pin_func(int pin, gpio_func func);

#endif