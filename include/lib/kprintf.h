#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdbool.h>

/**
 * Print a string with formatter.
*/
void kprintf(char* fmt_str, ...);

/**
 * Convert an int to a String.
*/
void itoa(int num, int base, bool signed_num, char* nums);

/**
 * Print an int with special formatting.
*/
void kprintf_int(char* nums, bool hex_prefix, bool field_flag, bool zeros_flag);

#endif
