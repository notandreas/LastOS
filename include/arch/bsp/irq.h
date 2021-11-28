#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#include <kernel/panic.h>

#include <config.h>

/**
 * @brief Set the up timer object
 * 
 */
void setup_timer();


/**
 * @brief Set the up uart object
 * 
 */
void setup_uart();


/**
 * @brief Reset the System-Timer.
 * 
 */
void reset_timer();

/**
 * @brief Get the pending 1 object
 * 
 * @return pending register irq[0, 31]
 */
int get_pending_1();

/**
 * @brief Get the pending 2 object
 * 
 * @return pending register irq[32, 63]
 */
int get_pending_2();

#endif
