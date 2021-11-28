#ifndef PANIC_H
#define PANIC_H

#include <stdarg.h>


/**
 * @brief The Panic handler. !!! PAAAAAAAAANIIIIIIC !!!
 * Panic message is send over uart and loop infinit.
 * 
 * @param file Current file name.
 * @param line Current line number.
 * @param show Toggele line and file print.
 * @param panic_text Panic text.
 * @param ... Varargs for the formatter.
 */
void panic_call(char* file, unsigned int line, int show, char* panic_text, ...);


/**
 * @brief Print panic without file name and line number.
 * 
 */
#define panic(panic_text, ...) panic_call(__FILE__, __LINE__, 0, panic_text, ##__VA_ARGS__);


/**
 * @brief Print panic with file name and line number.
 * 
 */
#define panic_info(panic_text, ...) panic_call(__FILE__, __LINE__, 1, panic_text, ##__VA_ARGS__);

#endif
