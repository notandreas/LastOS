#include <arch/bsp/yellow_led.h>
#include <lib/kprintf.h>

volatile unsigned int counter = 0;

void increment_counter() {
	counter++;
}


void start_kernel(){

	kprintf("Char: %c\n", 65);
	char* test = "Hello World";
	kprintf("String: %s\n", test);
	kprintf("Hex: %x\n", 1334);
	kprintf("Int: %i\n", -2147483648);
	kprintf("UInt: %u\n", 4294967295);
	kprintf("Pointer: %p\n", test);
	kprintf("%%");

	// Endless counter
	for (;;) {
		increment_counter();
	}
}
