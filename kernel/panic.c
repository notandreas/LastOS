#include <lib/kprintf.h>

void panic(char* panic_text) {
    kprintf("\n[\x1b[31mPANIC\x1b[0m] ");
    kprintf(panic_text);
    kprintf("\n");
    for (;;) {}
}