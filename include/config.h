#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Falls auf den Boards gearbeitet wird am besten
 * die nächste Zeile auskommentieren
 */
#define __QEMU__

/**
 * \file config.h
 *
 * Enthält defines und static Funktionen zum testen der
 * Implementierung. Wir tauschen diese Werte/Funktionen beim
 * Korrigieren zum Testen aus. Bitte fügt hier KEINE weiteren
 * defines oÄ ein. Ihr könnt diese Werte zum Testen natürlich
 * auch gerne selbst verändern.
 */

/* Include-Pfad bitte gegenfalls anpassen */
#include <lib/kprintf.h>
static inline void test_kprintf(void) {
	/* Nur für Aufgabenblatt 1
	 * Hier Test Funktion für kprintf schreiben und
	 * nach dem vollständigen initialisieren aufrufen
	 */
	kprintf("kprintf_test\n");

	kprintf("\n");
	kprintf("Char: %c\n", 65);
	char* test = "Hello World";
	kprintf("String: %s\n", test);
	kprintf("Hex: %x\n", 1334);
	kprintf("Int: %i\n", 0x80000000); //-2147483648
	kprintf("UInt: %u\n", 0xffffffff); //4294967295
	kprintf("Pointer: %p\n", test);
	kprintf("Percent: %%\n");
	kprintf("\n");
	kprintf("Test: %8u\n", 123);
	kprintf("Test: %8i\n", -12345);
	kprintf("Test: %8p\n", 0xff);
	kprintf("\n");
	kprintf("Test: %08u\n", 123);
	kprintf("Test: %08i\n", -12345);
	kprintf("Test: %08p\n", 0xff);
	kprintf("\n");
}

/**
 * Erst ab Aufgabenblatt 2 relevant
 */

#ifdef __QEMU__
/* Werte zum testen unter QEMU */
#define BUSY_WAIT_COUNTER 3000000
#else
/* Werte zum testen auf der Hardware */
#define BUSY_WAIT_COUNTER 30000
#endif // __QEMU__

// Wir testen nur mit Werten die durch 2^n darstellbar sind
#define UART_INPUT_BUFFER_SIZE 128

// Timer Interrupt Interval zum testen in Mikrosekunden
// Systimer taktet mit 1MHz
// 1000000 -> 1 Sekunde
#define TIMER_INTERVAL 1000000


#endif // _CONFIG_H_
