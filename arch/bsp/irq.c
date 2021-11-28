#include <arch/bsp/irq.h>

#define TIMER_BASE (0x7E003000 - 0x3F000000)
#define IRQ_REGISTER_BASE (0x7E00B200 - 0x3F000000)

typedef struct _timer_registers {
    uint32_t cs;
    uint32_t clo;
    uint32_t chi;
    uint32_t c0;
    uint32_t c1;
    uint32_t c2;
    uint32_t c3;
} timer;

static volatile timer * const timer_registers = (timer *) TIMER_BASE;

typedef struct _irq_table {
    uint32_t irq_basic_pending;
    uint32_t irq_pending_1;
    uint32_t irq_pending_2;
    uint32_t irq_control;
    uint32_t irq_enable_1;
    uint32_t irq_enable_2;
    uint32_t irq_basic_enable;
    uint32_t irq_disable_1;
    uint32_t irq_disable_2;
    uint32_t irq_basic_disable;
} irq_table;

static volatile irq_table * const irq_resisters = (irq_table *) IRQ_REGISTER_BASE;

void setup_timer() {
    timer_registers->cs = 1;
    timer_registers->c0 = timer_registers->clo + TIMER_INTERVAL;
    irq_resisters->irq_enable_1 |= 1;
}

void setup_uart() {
    irq_resisters->irq_enable_2 |= (1 << 25);
}

void reset_timer() {
    timer_registers->cs = 1;
    timer_registers->c0 = timer_registers->clo + TIMER_INTERVAL;
}

int get_pending_1() {
    return (irq_resisters->irq_pending_1);
}

int get_pending_2() {
    return (irq_resisters->irq_pending_2);
}
