#include <arch/bsp/interrupt.h>

void undefined_instruction_interrupt(arm_registers *reg) {
    kprintf("\n###########################################################################\n");
    kprintf("Undefined Instruction an Adresse: 0x%08x\n", (unsigned int) (reg->und_lr) - 4);

    print_registers(reg, reg->und_sp, reg->usr_lr);
    print_pcr(reg->cpsr, reg->und_spsr);
    print_mode_register(reg);
}


void software_interrupt_interrupt(arm_registers *reg) {
    kprintf("\n###########################################################################\n");
    kprintf("Software Interrupt an Adresse: 0x%08x\n", (unsigned int) (reg->svc_lr) - 4);

    print_registers(reg, reg->svc_sp, reg->svc_lr);
    print_pcr(reg->cpsr, reg->svc_spsr);
    print_mode_register(reg);
}


void prefetch_abort_interrupt(arm_registers *reg){
    kprintf("\n###########################################################################\n");
    kprintf("Prefetch Abort an Adresse: 0x%08x\n", (unsigned int) (reg->abt_lr) - 4);

    // read the ifar register
    uint32_t ifar_save;
    asm volatile ("mrc p15, 0, %0, c6, c0, 2"
      : "=r" (ifar_save)
    );

    // read the ifsr register
    uint32_t ifsr_save;
    asm volatile ("mrc p15, 0, %0, c5, c0, 1"
      : "=r" (ifsr_save)
    );

    kprintf("Instruktion: Adresse (0x%08x)\n", (unsigned int) ifar_save);
    kprintf("Fehler: ");
    print_ifsr(ifsr_save);
    kprintf("\n");

    print_registers(reg, reg->abt_sp, reg->abt_lr);
    print_pcr(reg->cpsr, reg->abt_spsr);
    print_mode_register(reg);
}


void data_abort_interrupt(arm_registers *reg) {
    kprintf("\n###########################################################################\n");
    kprintf("Data Abort an Adresse: 0x%08x\n", (unsigned int) (reg->abt_lr) - 8);

    // read the dfar register
    uint32_t dfar_save;
    asm volatile ("mrc p15, 0, %0, c6, c0, 0"
      : "=r" (dfar_save)
    );

    // read the dfsr register
    uint32_t dfsr_save;
    asm volatile ("mrc p15, 0, %0, c5, c0, 0"
      : "=r" (dfsr_save)
    );

    kprintf("Zugriff: %s auf Adresse (0x%08x)\n", (dfsr_save & (1 << 11)) ? "schreibend" : "lesend", (unsigned int) dfar_save);
    kprintf("Fehler: ");
    print_dfsr(dfsr_save);
    kprintf("\n");

    print_registers(reg, reg->abt_sp, reg->abt_lr);
    print_pcr(reg->cpsr, reg->abt_spsr);
    print_mode_register(reg);
}


void irq_interrupt(arm_registers *reg) {
    
    // check if the irq is a timer interrupt
    if (get_pending_1() & 1) {
        reset_timer();                  // reset the timer

        if (get_next() != 0) {
            kprintf("switch\n");
        }
        else {
            kprintf("No Thread to switch\n");
        }

        // print the '!\\n' if toggled
        if (toggle_irq_timer_print)
            kprintf("!\n");
    
        // print the register dump if toggled
        if (toggle_irq_dump_print == 1) {
            kprintf("\n###########################################################################\n");
            kprintf("IRQ Interrupt an Adresse: 0x%08x\n", (unsigned int) (reg->irq_lr) - 8);

            print_registers(reg, reg->irq_sp, reg->irq_lr);
            print_pcr(reg->cpsr, reg->irq_spsr);
            print_mode_register(reg);
        }
    }

    // check if the irq is a uart interrupt
    else if (get_pending_2() & (1 << 25)) {
        uart_add_to_buffer(uart_get_c());
    }
}


void fiq_interrupt(arm_registers *reg) {
    kprintf("\n###########################################################################\n");
    kprintf("FIQ Interrupt an Adresse: 0x%08x\n", (unsigned int) (reg->fiq_lr) - 8);

    print_registers(reg, reg->fiq_sp, reg->irq_lr);
    print_pcr(reg->cpsr, reg->fiq_spsr);
    print_mode_register(reg);
}


void print_registers(arm_registers *reg, uint32_t sp, uint32_t lr) {
    kprintf("\n>>> Registerschnappschuss (aktueller Modus) <<<\n");
    kprintf("R0: 0x%08x   R8:  0x%08x\n", (unsigned int) (reg->r0), (unsigned int) (reg->r8));
    kprintf("R1: 0x%08x   R9:  0x%08x\n", (unsigned int) (reg->r1), (unsigned int) (reg->r9));
    kprintf("R2: 0x%08x   R10: 0x%08x\n", (unsigned int) (reg->r2), (unsigned int) (reg->r10));
    kprintf("R3: 0x%08x   R11: 0x%08x\n", (unsigned int) (reg->r3), (unsigned int) (reg->r11));
    kprintf("R4: 0x%08x   R12: 0x%08x\n", (unsigned int) (reg->r4), (unsigned int) (reg->r12));
    kprintf("R5: 0x%08x   SP:  0x%08x\n", (unsigned int) (reg->r5), (unsigned int) (sp));
    kprintf("R6: 0x%08x   LR:  0x%08x\n", (unsigned int) (reg->r6), (unsigned int) (lr));
    kprintf("R7: 0x%08x   PC:  0x%08x\n", (unsigned int) (reg->r7), (unsigned int) (reg->pc));
}


void print_pcr(uint32_t cpsr, uint32_t spsr) {
    kprintf("\n>>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
    kprintf("CPSR: ");
    print_status(cpsr);
    kprintf("\n");
    kprintf("SPSR: ");
    print_status(spsr);
    kprintf("\n");
}


void print_mode_register(arm_registers *reg) {
    kprintf("\n>>> Aktuelle modusspezifische Register <<<\n");
    kprintf("             LR         SP         SPSR\n");
    kprintf("User/System: 0x%08x 0x%08x \n", (unsigned int) reg->usr_lr, (unsigned int) reg->usr_sp);
    kprintf("Supervisor:  0x%08x 0x%08x ", (unsigned int) reg->usr_lr, (unsigned int) reg->svc_sp);
    print_status(reg->svc_spsr);
    kprintf("\n");
    kprintf("Abort:       0x%08x 0x%08x ", (unsigned int) reg->abt_lr, (unsigned int) reg->abt_sp);
    print_status(reg->abt_spsr);
    kprintf("\n");
    kprintf("FIQ:         0x%08x 0x%08x ", (unsigned int) reg->fiq_lr, (unsigned int) reg->fiq_sp);
    print_status(reg->fiq_spsr);
    kprintf("\n");
    kprintf("IRQ:         0x%08x 0x%08x ", (unsigned int) reg->irq_lr, (unsigned int) reg->irq_sp);
    print_status(reg->irq_spsr);
    kprintf("\n");
    kprintf("Undefine:    0x%08x 0x%08x ", (unsigned int) reg->und_lr, (unsigned int) reg->und_sp);
    print_status(reg->und_spsr);
    kprintf("\n");
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

    // Print mode name for the psr (prs arm doc)
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

    // get the important bits (bsprak vl)
    status |= sr & 0b1111;
    status |= (sr & (1 << 10)) >> 6;

    // check for type (bsprak vl)
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

    // get the important bits (bsprak vl)
    status |= sr & 0b1111;
    status |= (sr & (1 << 9)) >> 6;

    // check for type (bsprak vl)
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
