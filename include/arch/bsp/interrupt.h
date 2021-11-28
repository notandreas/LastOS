#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#include <kernel/panic.h>

#include <lib/kprintf.h>
#include <arch/bsp/irq.h>
#include <arch/bsp/uart.h>

typedef struct _arm_registers {
    uint32_t cpsr;
    uint32_t usr_sp;
    uint32_t usr_lr;

    uint32_t svc_sp;
    uint32_t svc_lr;
    uint32_t svc_spsr;

    uint32_t abt_sp;
    uint32_t abt_lr;
    uint32_t abt_spsr;

    uint32_t fiq_sp;
    uint32_t fiq_lr;
    uint32_t fiq_spsr;

    uint32_t irq_sp;
    uint32_t irq_lr;
    uint32_t irq_spsr;

    uint32_t und_sp;
    uint32_t und_lr;
    uint32_t und_spsr;

    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t pc;
} arm_registers;

extern int toggle_irq_dump_print;

extern int toggle_irq_timer_print;

void print_registers(arm_registers *reg, uint32_t sp, uint32_t lr);

void print_pcr(uint32_t cpsr, uint32_t spsr);

void print_mode_register(arm_registers *reg);

void print_status(unsigned int psr);

void print_dfsr(unsigned int sr);

void print_ifsr(unsigned int sr);

#endif