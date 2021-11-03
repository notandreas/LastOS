#include <lib/kprintf.h>

void panic(char* panic_text) {
    kprintf("\n[PANIC] ");
    kprintf(panic_text);
    kprintf("\n");
    for (;;) {}
}