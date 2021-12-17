#ifndef KTHREAD_H
#define KTHREAD_H

#include <kernel/panic.h>

#include <lib/kprintf.h>
#include <config.h>

void kthread_start(int thread_id);

void kthread_create(void (*func)(void*), const void *args, unsigned int args_size);

#endif