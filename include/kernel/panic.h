#ifndef PANIC_H
#define PANIC_H

#include <stdarg.h>

/**
 * PAAAAAAAAANIIIIIIC!!!
 * If panic a message is send over uart and loop infinit.
*/
void panic_call(char* file, unsigned int line, int show, char* panic_text, ...);

#define panic(panic_text, ...) panic_call(__FILE__, __LINE__, 0, panic_text, ##__VA_ARGS__);
#define panic_info(panic_text, ...) panic_call(__FILE__, __LINE__, 1, panic_text, ##__VA_ARGS__);

#endif
