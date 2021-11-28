#include <arch/bsp/interrupt.h>
#include <arch/bsp/irq.h>

#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

#include <arch/bsp/regcheck.h>

#include <config.h>

int toggle_irq_dump_print = 0;
int toggle_irq_timer_print = 0;

void __attribute__((optimize("O0"))) sub_programm() {
    for (unsigned int i = 0; i < INPUT_WAIT_COUNTER; i++) {}
    do {
        while (uart_has_next()) {
            char c = uart_next();
            for (int i = 0; i < WORK_COUNTER; i++) {
                kprintf("%c", c);
                for (unsigned int i = 0; i < BUSY_WAIT_COUNTER; i++) {}
            }
        }

        for (unsigned int i = 0; i < INPUT_WAIT_COUNTER; i++) {}
    } while (uart_has_next());
}

void start_kernel(){
    setup_timer();

    init_uart();

    setup_uart();

    for (;;) {
        if (uart_has_next()) {
            switch (uart_next()) {
                case 's':
                    asm volatile ("svc #0");
                    break;
                case 'p':
                    asm volatile ("bkpt");
                    break;
                case 'a':
                    asm volatile ("mov r0, #1\n" "ldr r0, [r0]");
                    break;
                case 'u':
                    asm volatile ("udf");
                    break;
                case 'd':
                    toggle_irq_dump_print = toggle_irq_dump_print ? 0 : 1;
                    break;
                case 'e':
                    toggle_irq_timer_print = 1;
                    sub_programm();
                    toggle_irq_timer_print = 0;
                    break;
                case 'c':
                    register_checker();
                    break;
                default:
                    break;
            }
        }
    }
}
