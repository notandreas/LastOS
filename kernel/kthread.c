#include <kernel/kthread.h>

static tcb_list_elem thread_list[THREAD_COUNT];
static int thread_count = 0;
static tcb_list_elem kernel_thread;
static int tcb_current = -1;


int get_current_thread() {
    return tcb_current;
}


tcb_list_elem* get_next() {
    if (tcb_current == -1)
        return kernel_thread.next;
    else {
        if (thread_list[tcb_current].next == 0)
            return &kernel_thread;
        else
            return thread_list[tcb_current].next;
    }
}


void kthread_swap(arm_registers *regs) {
    if (tcb_current == -1 && kernel_thread.next != 0) {
        kthread_store(regs, &kernel_thread);
        tcb_current = kernel_thread.next->id;
        kthread_load(regs, (thread_list + tcb_current));
    }
    else if (thread_list[tcb_current].next != 0) {
        kthread_store(regs, (thread_list + tcb_current));
        tcb_current = thread_list[tcb_current].next->id;
        kthread_load(regs, (thread_list + tcb_current));
    }
    else if (thread_list[tcb_current].next == 0) {
        kthread_store(regs, (thread_list + tcb_current));
        tcb_current = -1;
        kthread_load(regs, &kernel_thread);
    }
}


void kthread_store(arm_registers *regs, tcb_list_elem* t_save) {
    t_save->saved_thread.r0 = regs->r0;
    t_save->saved_thread.r1 = regs->r1;
    t_save->saved_thread.r2 = regs->r2;
    t_save->saved_thread.r3 = regs->r3;
    t_save->saved_thread.r4 = regs->r4;
    t_save->saved_thread.r5 = regs->r5;
    t_save->saved_thread.r6 = regs->r6;
    t_save->saved_thread.r7 = regs->r7;
    t_save->saved_thread.r8 = regs->r8;
    t_save->saved_thread.r9 = regs->r9;
    t_save->saved_thread.r10 = regs->r10;
    t_save->saved_thread.r11 = regs->r11;
    t_save->saved_thread.r12 = regs->r12;
    t_save->saved_thread.sp = regs->usr_sp;
    t_save->saved_thread.lr = regs->usr_lr;
    t_save->saved_thread.pc = regs->irq_lr;
    t_save->saved_thread.cpsr = regs->irq_spsr;
}


void kthread_load(arm_registers *regs, tcb_list_elem* t_save) {
    regs->r0 = t_save->saved_thread.r0;
    regs->r1 = t_save->saved_thread.r1;
    regs->r2 = t_save->saved_thread.r2;
    regs->r3 = t_save->saved_thread.r3;
    regs->r4 = t_save->saved_thread.r4;
    regs->r5 = t_save->saved_thread.r5;
    regs->r6 = t_save->saved_thread.r6;
    regs->r7 = t_save->saved_thread.r7;
    regs->r8 = t_save->saved_thread.r8;
    regs->r9 = t_save->saved_thread.r9;
    regs->r10 = t_save->saved_thread.r10;
    regs->r11 = t_save->saved_thread.r11;
    regs->r12 = t_save->saved_thread.r12;

    regs->irq_lr = t_save->saved_thread.pc;
}


void kthread_create(void (*func)(void*), const void *args, unsigned int args_size) {
    if (thread_count < THREAD_COUNT) {
        for (int i = 0; i < THREAD_COUNT; i++) {
            if (thread_list[i].in_use != 0)
                continue;

            tcb_list_elem *tmp = &kernel_thread;
            while (tmp->next != 0) {
                tmp = tmp->next;
            }

            tmp->next = thread_list + i;
            thread_list[i].in_use = 1;
            thread_list[i].id = i;
            thread_list[i].saved_thread.pc = (int) func + 4;
            thread_list[i].saved_thread.lr = (int) kthread_exit + 4;
            thread_list[i].saved_thread.sp = 0x1337;
            break;
        }
        thread_count++;
    }
    else
        panic("Max thread count reached! %i/%i\n", thread_count, THREAD_COUNT);
}

void kthread_exit() {
    kprintf("EXIT %i\n", tcb_current);
}
