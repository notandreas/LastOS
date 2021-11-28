#include <arch/bsp/interrupt.h>
#include <arch/bsp/irq.h>

#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

#include <arch/bsp/regcheck.h>

#include <config.h>

int toggle_irq_dump_print = 0;
int toggle_irq_timer_print = 0;


/**
 * @brief The "Sub Program"
 * 
 */
void __attribute__((optimize("O0"))) sub_program() {                                // disable gcc opt for this function
    for (unsigned int i = 0; i < INPUT_WAIT_COUNTER; i++) {}                        // Waiting time for input. (setting in config.h)
    int wait_more = 0;                                                              // var to check if need more time after first input.

    do {
        while (uart_has_next()) {                                                   // Checks if ringbuffer is empty. Loop n times, if contanes n chars.
            wait_more = 1;

            char c = uart_next();                                                   // Get FIFO char from ringbuffer.
            for (int i = 0; i < WORK_COUNTER; i++) {                                // Prints WORK_COUNTER times the char with delay. (setting in config.h)
                kprintf("%c", c);
                for (unsigned int i = 0; i < BUSY_WAIT_COUNTER; i++) {}             // Waiting time between prints. ("work time")
            }
        }

        if (wait_more)                                                              // Only if in the first wait chars are typed.
            for (unsigned int i = 0; i < INPUT_WAIT_COUNTER; i++) {}                // Waitin time, if more chars are in comming.
    } while (uart_has_next());                                                      // Run only one time, but if has more chars to print try it.
}

void start_kernel(){
    setup_timer();                                                                  // setup timer interrupts.

    init_uart();                                                                    // setup uart.
    setup_uart();                                                                   // setup the uart interrupts.

    for (;;) {
        if (uart_has_next()) {                                                      // check if ringbuffer contanes input.
            switch (uart_next()) {
                case 's':
                    asm volatile ("svc #0");                                        // trigger svc.
                    break;
                case 'p':
                    asm volatile ("bkpt");                                          // trigger prefetch abt.
                    break;
                case 'a':
                    asm volatile ("ldr r0, [sp, #1]");                              // trigger data abt.
                    break;
                case 'u':
                    asm volatile ("udf");                                           // trigger undef instr.
                    break;
                case 'd':
                    toggle_irq_dump_print = toggle_irq_dump_print ? 0 : 1;          // toggle the print of irq register dump.
                    break;
                case 'e':
                    toggle_irq_timer_print = 1;                                     // toggle the print of '!\\n'. (enable)
                    sub_program();                                                  // run sub program.
                    toggle_irq_timer_print = 0;                                     // toggle the print of '!\\n'. (disable)
                    break;
                case 'c':
                    register_checker();                                             // check register if correctly restored.
                    break;
                default:
                    break;
            }
        }
    }
}
