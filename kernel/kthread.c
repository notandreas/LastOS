#include <kernel/kthread.h>

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

static tcb thread_list[THREAD_COUNT] = {0};
static int thread_exist[THREAD_COUNT] = {0};
static int tcb_current_thread = -1;


void kthread_start(int thread_id) {
    if (thread_exist[thread_id] == 1) {

    }
    else
        panic("Thread with ID %i not exist!", thread_id);
}


void kthread_create(void (*func)(void*), const void *args, unsigned int args_size){
}
