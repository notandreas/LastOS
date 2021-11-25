#include <stdint.h>

#include <kernel/panic.h>

#include <arch/bsp/interrupt.h>
#include <lib/kprintf.h>

void undefined_instruction_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]) {
    kprintf("\n###########################################################################\n");
    kprintf("Undefined Instruction an Adresse: 0x%08x\n", (unsigned int) reg[14] - 4);

    print_registers(reg);
    print_pcr(psr);
}

void software_interrupt_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]) {
    kprintf("\n###########################################################################\n");
    kprintf("Software Interrupt an Adresse: 0x%08x\n", (unsigned int) reg[14] - 4);

    print_registers(reg);
    print_pcr(psr);
}

void prefetch_abort_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]){
    kprintf("\n###########################################################################\n");
    kprintf("Prefetch Abort an Adresse: 0x%08x\n", (unsigned int) reg[14] - 4);

    uint32_t ifar_save;
    asm("mrc p15, 0, %0, c6, c0, 2"
      : "=r" (ifar_save)
    );

    uint32_t ifsr_save;
    asm("mrc p15, 0, %0, c5, c0, 1"
      : "=r" (ifsr_save)
    );

    kprintf("Instruktion: Adresse (0x%08x)\n", (unsigned int) ifar_save);
    kprintf("Fehler: ");
    print_ifsr(ifsr_save);
    kprintf("\n");

    print_registers(reg);
    print_pcr(psr);
}

void data_abort_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]) {
    kprintf("\n###########################################################################\n");
    kprintf("Data Abort an Adresse: 0x%08x\n", (unsigned int) reg[14] - 8);

    uint32_t dfar_save;
    asm("mrc p15, 0, %0, c6, c0, 0"
      : "=r" (dfar_save)
    );

    uint32_t dfsr_save;
    asm("mrc p15, 0, %0, c5, c0, 0"
      : "=r" (dfsr_save)
    );

    kprintf("Zugriff: %s auf Adresse (0x%08x)\n", (dfsr_save & (1 << 11)) ? "schreibend" : "lesend", (unsigned int) dfar_save);
    kprintf("Fehler: ");
    print_dfsr(dfsr_save);
    kprintf("\n");

    print_registers(reg);
    print_pcr(psr);
}

void irq_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]) {
    /*
        kprintf("\n###########################################################################\n");
        kprintf("IRQ Interrupt an Adresse: 0x%08x\n", (unsigned int) reg[14] - 8);

        print_registers(reg);
        print_pcr(psr);
    */
}

void fiq_interrupt(uint32_t reg[16], uint32_t psr[2], uint32_t mod_reg[14]) {
    kprintf("\n###########################################################################\n");
    kprintf("FIQ Interrupt an Adresse: 0x%08x\n", (unsigned int) reg[14] - 8);

    print_registers(reg);
    print_pcr(psr);
}

void print_registers(uint32_t reg[16]) {
    kprintf("\n>>> Registerschnappschuss (aktueller Modus) <<<\n");
    kprintf("R0: 0x%08x   R8:  0x%08x\n", (unsigned int) reg[0], (unsigned int) reg[8]);
    kprintf("R1: 0x%08x   R9:  0x%08x\n", (unsigned int) reg[1], (unsigned int) reg[9]);
    kprintf("R2: 0x%08x   R10: 0x%08x\n", (unsigned int) reg[2], (unsigned int) reg[10]);
    kprintf("R3: 0x%08x   R11: 0x%08x\n", (unsigned int) reg[3], (unsigned int) reg[11]);
    kprintf("R4: 0x%08x   R12: 0x%08x\n", (unsigned int) reg[4], (unsigned int) reg[12]);
    kprintf("R5: 0x%08x   SP:  0x%08x\n", (unsigned int) reg[5], (unsigned int) reg[13]);
    kprintf("R6: 0x%08x   LR:  0x%08x\n", (unsigned int) reg[6], (unsigned int) reg[14]);
    kprintf("R7: 0x%08x   PC:  0x%08x\n", (unsigned int) reg[7], (unsigned int) reg[15]);
}

void print_pcr(uint32_t psr[2]) {
    kprintf("\n>>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
    kprintf("CPSR: ");
    print_status(psr[0]);
    kprintf("\n");
    kprintf("SPSR: ");
    print_status(psr[1]);
    kprintf("\n");
}

void print_mode_register() {
    /*kprintf("\n>>> Aktuelle modusspezifische Register <<<\n");
    kprintf("             LR         SP         SPSR\n");
    kprintf("User/System: 0x%08x 0x%08x \n", usr[1], usr[0]);
    kprintf("Supervisor:  0x%08x 0x%08x ", svc[1], svc[0]);
    status_string(svc[4]);
    kprintf("\n");
    kprintf("Abort:       0x%08x 0x%08x ", reg[14], reg[13]);
    status_string(psr[1]);
    kprintf("\n");
    kprintf("FIQ:         0x%08x 0x%08x ", mod_reg[6], mod_reg[5]);
    status_string(mod_reg[7]);
    kprintf("\n");
    kprintf("IRQ:         0x%08x 0x%08x ", mod_reg[9], mod_reg[8]);
    status_string(mod_reg[10]);
    kprintf("\n");
    kprintf("Undefine:    0x%08x 0x%08x ", mod_reg[12], mod_reg[11]);
    status_string(mod_reg[13]);
    kprintf("\n");*/
}

void print_status(unsigned int psr) {
    unsigned char status = 0b00000000;
    char chars[] = {'N','Z','C','V','E', 'I', 'F', 'T'};    //chars im pool

    status |= (psr >> 28) << 4;                             // set the first 4 bits (NZCV)   

    status |= ((psr >> 9) & 1) << 3;                        // set the endian bit (E)

    status |= ((psr >> 5) & 7);                             // set the maskbits und thumb bit (IFT) 

    for(int i = 7; i >= 0; i--) {                           // print the number
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
            kprintf("            ");
            break;
    }

    kprintf(" (0x%08x)", psr);
}

void print_dfsr(unsigned int sr) {
    unsigned char status = 0;

    status |= sr & 0b1111;
    status |= (sr & (1 << 10)) >> 6;

    switch (status) {
        case 0b00000:
            kprintf("reset value");
            break;
        case 0b00001:
            kprintf("Alignment fault");
            break;
        case 0b00010:
            kprintf("Debug event fault");
            break;
        case 0b00011:
            kprintf("Access Flag fault on Section");
            break;
        case 0b00100:
            kprintf("Cache maintenance operation fault");
            break;
        case 0b00101:
            kprintf("Translation fault on Section");
            break;
        case 0b00110:
            kprintf("Access Flag fault on Page");
            break;
        case 0b00111:
            kprintf("Translation fault on Page");
            break;
        case 0b01000:
            kprintf("Precise External Abort");
            break;
        case 0b01001:
            kprintf("Domain fault on Section");
            break;
        case 0b01100:
            kprintf("External abort on Section");
            break;
        case 0b01101:
            kprintf("Permission fault on Section");
            break;
        case 0b01110:
            kprintf("External abort on Page");
            break;
        case 0b01111:
            kprintf("Permission fault on Page");
            break;
        case 0b10110:
            kprintf("Imprecise External Abort");
            break;
        default:
            kprintf("No function");
    }
}

void print_ifsr(unsigned int sr) {
    unsigned char status = 0;

    status |= sr & 0b1111;
    status |= (sr & (1 << 9)) >> 6;

    switch (status) {
        case 0b00000:
            kprintf("reset value");
            break;
        case 0b00010:
            kprintf("Debug event fault");
            break;
        case 0b00011:
            kprintf("Access Flag fault on Section");
            break;
        case 0b00101:
            kprintf("Translation fault on Section");
            break;
        case 0b00110:
            kprintf("Access Flag fault on Page");
            break;
        case 0b00111:
            kprintf("Translation fault on Page");
            break;
        case 0b01000:
            kprintf("Precise External Abort");
            break;
        case 0b01001:
            kprintf("Domain fault on Section");
            break;
        case 0b01011:
            kprintf("Domain fault on Page");
            break;
        case 0b01100:
            kprintf("External abort on Section");
            break;
        case 0b01101:
            kprintf("Permission fault on Section");
            break;
        case 0b01110:
            kprintf("External abort on Page");
            break;
        case 0b01111:
            kprintf("Permission fault on Page");
            break;
        default:
            kprintf("No function");
    }
}
