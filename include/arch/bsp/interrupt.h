#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#include <kernel/panic.h>

#include <kernel/kthread.h>
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

/**
 * @brief Globale variable, to let the irq print a register dump on every tick.
 * 
 */
extern int toggle_irq_dump_print;

/**
 * @brief Globale variable, to let the irq print a '!\\n' on every tick.
 * 
 */
extern int toggle_irq_timer_print;


/**
 * @brief Print all registers of an arm_registers struct.
 * 
 * @param reg Use only r0-r12 and pc.
 * @param sp The current sp of the mode.
 * @param lr The current lr of the mode. (return address without subtraction)
 */
void print_registers(arm_registers *reg, uint32_t sp, uint32_t lr);


/**
 * @brief Print a two row table with in a format like NZCV E IFT <MODE>.
 * 
 * @param cpsr the Current Program Status Register
 * @param spsr the Saved Program Status Registers
 */
void print_pcr(uint32_t cpsr, uint32_t spsr);


/**
 * @brief Print a table of ARM modes, with the current lr, sp and spsr.
 * 
 * @param reg A arm_registers Struct that contais all registers.
 */
void print_mode_register(arm_registers *reg);


/**
 * @brief Print the cpsr/spsr in a format like NZCV E IFT <MODE>.
 * 
 * @param psr The cpsr or the spsr
 */
void print_status(unsigned int psr);


/**
 * @brief Print the error of the data_abt, by decoding the dfsr.
 * 
 * @param sr The dfsr content to decode
 */
void print_dfsr(unsigned int sr);


/**
 * @brief Print the error of the prefetch_abt, by decoding the ifsr
 * 
 * @param sr The ifsr content to decode
 */
void print_ifsr(unsigned int sr);

#endif