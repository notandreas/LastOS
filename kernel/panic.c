#include<kernel/panic.h>
#include <lib/kprintf.h>

void panic_call(char* file, unsigned int line, int show, char* panic_text, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, panic_text);
    
    kprintf("\n[\x1b[31mPANIC\x1b[0m] ");
    va_printf(panic_text, arg_ptr);
    if (show)
        kprintf(" in %s:%u\n", file, line);

    va_end(arg_ptr);
    for (;;) {}
}