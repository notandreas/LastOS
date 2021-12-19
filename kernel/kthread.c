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


void kthread_swap() {
    if (tcb_current == -1 && kernel_thread.next != 0) {
        tcb_current = kernel_thread.next->id;
    }
    else if (thread_list[tcb_current].next != 0) {
        tcb_current = thread_list[tcb_current].next->id;
    }
    else if (thread_list[tcb_current].next == 0) {
        tcb_current = -1;
    }
}


void kthread_create(void (*func)(void*), const void *args, unsigned int args_size) {
    if (thread_count < THREAD_COUNT) {
        for (int i = 0; i < THREAD_COUNT; i++) {
            if (thread_list[i].in_use != 0)
                continue;

            tcb_list_elem *tmp = &kernel_thread.next;
            while (tmp->next != 0) {
                tmp = tmp->next;
            }

            tmp->next = thread_list + i;
            thread_list[i].in_use = 1;
            thread_list[i].id = i;
            break;
        }
        thread_count++;
    }
    else
        panic("Max thread count reached! %i/%i\n", thread_count, THREAD_COUNT);
}
