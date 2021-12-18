#include <kernel/kthread.h>

tcb_list_elem thread_list[THREAD_COUNT];
int thread_count = 0;
tcb_list_elem kernel_thread;
int tcb_current = -1;


int get_current_thread() {
    return tcb_current;
}


tcb_list_elem* get_next() {
    if (tcb_current == -1)
        return kernel_thread.next;
    else
        return thread_list[tcb_current].next;
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
            break;
        }
        thread_count++;
    }
    else
        panic("Max thread count reached! %i/%i", thread_count, THREAD_COUNT);
}
