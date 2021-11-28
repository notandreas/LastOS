#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#include <kernel/panic.h>

#include <config.h>

void setup_timer();

void setup_uart();

void reset_timer();

int get_pending_1();

int get_pending_2();

#endif
