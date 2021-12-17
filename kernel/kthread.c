#include <kernel/kthread.h>

typedef struct _kthread_save {
    arm_registers save;
    const void *arg;
    unsigned int args_s;
} kthread_save;

static kthread_save thread_list[THREAD_COUNT];

void kthread_start(int thread_id) {
}

void kthread_create(void (*func)(void*), const void *args, unsigned int args_size){
}
