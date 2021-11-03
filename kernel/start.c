#include <arch/bsp/uart.h>
#include <lib/kprintf.h>
#include <config.h>


void start_kernel(){

	init_uart();

	test_kprintf();

	for (;;) {
		char a = uart_get_c();
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08u\n", a, a, a);
	}
}
