#include <stdint.h>

#include <kernel/panic.h>

#include <arch/bsp/interrupt.h>
#include <lib/kprintf.h>

void printRegister(uint32_t cpsr, uint32_t spsr, uint32_t sp, uint32_t pc) {
    kprintf("\ncpsr: %x, spsr: %x, sp: %x, pc: %x\n", (unsigned int)cpsr, (unsigned int)spsr, (unsigned int)sp, (unsigned int)pc);
}

void status_string(unsigned int psr) {
    unsigned char status = 0b00000000;
    char chars[] = {'N','Z','C','V','E', 'I', 'F', 'T'};    //chars im pool

    status |= (psr >> 28) << 4;         // set the first 4 bits (NZCV)   

    status |= ((psr >> 9) & 1) << 3;    // set the endian bit (E)

    status |= ((psr >> 5) & 7);         // set the maskbits und thumb bit (IFT) 

    for(int i = 7; i >= 0; i--) {       // print the number
        if(i == 3 || i == 2)
            kprintf(" ");

        if((1 << i) & status)
            kprintf("%c", chars[7-i]);
        else
            kprintf("_");
    }

    kprintf(" ");

    switch (psr & 0x1F) {
        case 0x10:
            kprintf("User        ");
            break;
        case 0x11:
            kprintf("FIQ         ");
            break;
        case 0x12:
            kprintf("IRQ         ");
            break;
        case 0x13:
            kprintf("Supervisor  ");
            break;
        case 0x17:
            kprintf("Abort       ");
            break;
        case 0x1b:
            kprintf("Undefined   ");
            break;
        case 0x1F:
            kprintf("System      ");
            break;
        default:
            break;
    }

    kprintf(" (0x%08x)", psr);
}

void undefined_instruction_interrupt() {
    kprintf("foo1");
}

void software_interrupt_interrupt() {
    kprintf("foo2");
}

void prefetch_abort_interrupt(uint32_t reg[16], uint32_t psr[2]){
    kprintf("\n###########################################################################\n");
    kprintf("Prefetch Abort an Adresse: 0x%08x\n", (unsigned int) reg[14] - 4);
    kprintf("Instruktion:\n");
    kprintf("Fehler:\n");
    kprintf("\n>>> Registerschnappschuss (aktueller Modus) <<<\n");
    kprintf("R0: 0x%08x   R8:  0x%08x\n", (unsigned int) reg[0], (unsigned int) reg[8]);
    kprintf("R1: 0x%08x   R9:  0x%08x\n", (unsigned int) reg[1], (unsigned int) reg[9]);
    kprintf("R2: 0x%08x   R10: 0x%08x\n", (unsigned int) reg[2], (unsigned int) reg[10]);
    kprintf("R3: 0x%08x   R11: 0x%08x\n", (unsigned int) reg[3], (unsigned int) reg[11]);
    kprintf("R4: 0x%08x   R12: 0x%08x\n", (unsigned int) reg[4], (unsigned int) reg[12]);
    kprintf("R5: 0x%08x   SP:  0x%08x\n", (unsigned int) reg[5], (unsigned int) reg[13]);
    kprintf("R6: 0x%08x   LR:  0x%08x\n", (unsigned int) reg[6], (unsigned int) reg[14]);
    kprintf("R7: 0x%08x   PC:  0x%08x\n", (unsigned int) reg[7], (unsigned int) reg[15]);
    kprintf("\n>>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
    kprintf("CPSR: ");
    status_string(psr[0]);
    kprintf("\n");
    kprintf("SPSR: ");
    status_string(psr[1]);
    kprintf("\n");
    kprintf("\n>>> Aktuelle modusspezifische Register <<<\n");
    kprintf("             LR         SP         SPSR\n");
    kprintf("User/System: 0x00000000 0x00000000 \n");
    kprintf("Supervisor:  0x00000000 0x00000000 NZCV E IFT\n");
    kprintf("Abort:       0x00000000 0x00000000 NZCV E IFT\n");
    kprintf("FIQ:         0x00000000 0x00000000 NZCV E IFT\n");
    kprintf("IRQ:         0x00000000 0x00000000 NZCV E IFT\n");
    kprintf("Undefine:    0x00000000 0x00000000 NZCV E IFT\n");
    panic("Alles OK");
}

void data_abort_interrupt() {
    kprintf("foo3");
}

void irq_interrupt() {
    kprintf("foo4");
}

void fiq_interrupt() {
    kprintf("foo5");
}
