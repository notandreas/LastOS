#include <kernel/kthread.h>

static tcb_list_elem thread_list[THREAD_COUNT] = {0};
static int thread_count = 0;
static tcb_list_elem kernel_thread;
static int tcb_current = -1;


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
        thread_count++;
    }
    else
        panic("Max thread count reached! %i/%i", thread_count, THREAD_COUNT);
}
