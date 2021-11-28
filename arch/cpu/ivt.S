.macro store_all
    stmfd sp!, {r0-r12, pc}
    mov r1, sp
    sub r1, r1, #56
.endm

.macro load_all r_lr
    add sp, sp, #12
    add sp, sp, #(12 * \r_lr)
    ldr lr, [sp, #4]
    add sp, sp, #(12 * (5-\r_lr))
    ldmfd sp!, {r0-r12}
    add sp, sp, #4
.endm

.macro store_usr
    mrs r2, sp_usr
    mrs r3, lr_usr
    stmfd sp!, {r2-r3}
.endm

.macro store_und
    mrs r2, sp_und
    mrs r3, lr_und
    mrs r4, spsr_und
    stmfd sp!, {r2-r4}
.endm

.macro store_svc
    mrs r2, sp_svc
    mrs r3, lr_svc
    mrs r4, spsr_svc
    stmfd sp!, {r2-r4}
.endm

.macro store_abt
    mrs r2, sp_abt
    mrs r3, lr_abt
    mrs r4, spsr_abt
    stmfd sp!, {r2-r4}
.endm

.macro store_irq
    mrs r2, sp_irq
    mrs r3, lr_irq
    mrs r4, spsr_irq
    stmfd sp!, {r2-r4}
.endm

.macro store_fiq
    mrs r2, sp_fiq
    mrs r3, lr_fiq
    mrs r4, spsr_fiq
    stmfd sp!, {r2-r4}
.endm

.global _ivt
.balign 32

_ivt:
    b reset
    ldr pc, =undefined_instruction
    ldr pc, =software_interrupt
    ldr pc, =prefetch_abort
    ldr pc, =data_abort
    ldr pc, =not_used
    ldr pc, =irq
    ldr pc, =fiq

reset:

undefined_instruction:
    store_all
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_irq
    store_fiq
    store_abt
    store_svc
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl undefined_instruction_interrupt
    load_all 4
    subs pc, lr, #0

software_interrupt:
    store_all
    store_und
    store_irq
    store_fiq
    store_abt
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl software_interrupt_interrupt
    load_all 0
    subs pc, lr, #0

prefetch_abort:
    store_all
    store_und
    store_irq
    store_fiq
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_svc
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl prefetch_abort_interrupt
    load_all 1
    subs pc, lr, #0

data_abort:
    store_all
    store_und
    store_irq
    store_fiq
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_svc
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl data_abort_interrupt
    load_all 1
    subs pc, lr, #4

irq:
    store_all
    store_und
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_fiq
    store_abt
    store_svc
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl irq_interrupt
    load_all 3
    subs pc, lr, #4

fiq:
    store_all
    store_und
    store_irq
    mov r2, lr
    mrs r3, spsr
    stmfd sp!, {r1-r3}
    store_abt
    store_svc
    store_usr
    mrs r0, cpsr
    stmfd sp!, {r0}
    mov r0, sp
    bl fiq_interrupt
    load_all 2
    subs pc, lr, #4

not_used:
