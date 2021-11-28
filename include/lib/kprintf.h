#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdbool.h>
#include <stdarg.h>

#include <kernel/panic.h>

#include <arch/bsp/uart.h>

/**
 * @brief Logic to print a string with a formatter.
 * 
 * @param fmt_str String with formatter.
 * @param arg_ptr Varargs for the formatter.
 */
void va_printf(char* fmt_str, va_list arg_ptr);

/**
 * @brief Call the va_printf(); and hanle warnings.
 * 
 */
void kprintf(char* fmt_str, ...) __attribute__((format(printf, 1, 2)));

/**
 * @brief Convert an int to a String.
 * 
 * @param num The Number to convert.
 * @param base Number base e.g. 10 for Base 10. (-/+) only works for Base 10.
 * @param signed_num If (num) need to be interpreted as signed. Relevant for Base 10.
 * @param nums Number arry contaning the digits to print.
 */
void itoa(int num, int base, bool signed_num, char* nums);

/**
 * @brief Print an int with special formatting.
 * 
 * @param nums The Number digits to print
 * @param hex_prefix If true, the 0x prefix is printed.
 * @param field_flag If true, number is filled with space.
 * @param zeros_flag If true, the space fill is replaced with 0.
 */
void kprintf_int(char* nums, bool hex_prefix, bool field_flag, bool zeros_flag);

#endif
