#include <arch/bsp/uart.h>
#include <lib/kprintf.h>
#include <config.h>


void start_kernel(){

    init_uart();

    for (;;) {
        char in_char = uart_get_c();

        if (in_char == 's'){
            asm("svc #0");
        }
        else if (in_char == 'p') {
            asm("bkpt #0");
        }
        else if (in_char == 'a') {
            asm("ldr r0, [sp, #1]");
        }
        else if (in_char == 'u') {
            asm("udf #0");
        }
    }
}
