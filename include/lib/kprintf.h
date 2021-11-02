#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdbool.h>

void kprintf(char* fmt_str, ...);

void itoa(int num, int base, bool signed_num, char* nums);

void kprintf_int(char* nums);

#endif
