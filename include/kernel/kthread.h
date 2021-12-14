#ifndef KTHREAD_H
#define KTHREAD_H

extern int thread_list[32];

void thread_start();

void thread_create(void (*func)(void*), const void *args, unsigned int args_size);

#endif