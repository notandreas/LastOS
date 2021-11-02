#include <arch/bsp/uart.h>
#include <lib/kprintf.h>


void start_kernel(){

	kprintf("Char: %c\n", 65);
	char* test = "Hello World";
	kprintf("String: %s\n", test);
	kprintf("Hex: %x\n", 1334);
	kprintf("Int: %i\n", -2147483648);
	kprintf("UInt: %u\n", 4294967295);
	kprintf("Pointer: %p\n", test);
	kprintf("%%\n");

	// Endless counter
	for (;;) {
		char a = uart_get_c();
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %u\n", a, a, a);
	}
}
