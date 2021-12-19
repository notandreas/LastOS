#ifndef KTHREAD_H
#define KTHREAD_H

#include <kernel/panic.h>

#include <arch/bsp/interrupt.h>
#include <lib/kprintf.h>
#include <config.h>

typedef enum _thread_status {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} thread_status;


typedef struct _tcb {
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
    uint32_t sp;
    uint32_t lr;
    uint32_t pc;
    uint32_t cpsr;
    thread_status t_status;
} tcb;

typedef struct _tcb_list_elem {
    struct _tcb_list_elem *next;
    int in_use;
    int id;
    tcb saved_thread;
} tcb_list_elem;


int get_current_thread();

tcb_list_elem* get_next();

void kthread_swap(arm_registers *regs);

void kthread_store(arm_registers *regs, tcb_list_elem* t_save);

void kthread_load(arm_registers *regs, tcb_list_elem* t_save);

void kthread_create(void (*func)(void*), const void *args, unsigned int args_size);

void kthread_exit();

#endif