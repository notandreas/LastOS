#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdbool.h>
#include <stdarg.h>

#include <kernel/panic.h>

#include <arch/bsp/uart.h>

/**
 * Logic to print a string with a formatter.
*/
void va_printf(char* fmt_str, va_list arg_ptr);

/**
 * Call the va_printf(); and hanle warnings.
*/
void kprintf(char* fmt_str, ...) __attribute__((format(printf, 1, 2)));

/**
 * Convert an int to a String.
*/
void itoa(int num, int base, bool signed_num, char* nums);

/**
 * Print an int with special formatting.
*/
void kprintf_int(char* nums, bool hex_prefix, bool field_flag, bool zeros_flag);

#endif
