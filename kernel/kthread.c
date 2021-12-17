#include <kernel/kthread.h>

static int thread_list[THREAD_COUNT];

void kthread_start(int thread_id) {
}

void kthread_create(void (*func)(void*), const void *args, unsigned int args_size){
}
