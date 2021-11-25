.macro store_all
    stmfd sp!, {r0-r12, sp, lr, pc}
    mov r0, sp

    mrs r1, spsr
    str r1, [sp, #-4]
    mrs r1, cpsr
    str r1, [sp, #-8]
    sub sp, #8
    mov r1, sp
.endm

.macro load_all
    add sp, sp, #56
    add sp, sp, #8
    ldmfd sp!, {r0-r12}
    ldr lr, [sp, #4]
    add sp, sp, #12
.endm

.macro store_usr
    mrs r3, lr_usr
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_usr
    str r3, [sp, #-4]
    sub sp, sp, #4
.endm

.macro store_und
    mrs r3, spsr_und
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, lr_und
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_und
    str r3, [sp, #-4]
    sub sp, sp, #4
.endm

.macro store_svc
    mrs r3, spsr_svc
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, lr_svc
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_svc
    str r3, [sp, #-4]
    sub sp, sp, #4
.endm

.macro store_abt
    mrs r3, spsr_abt
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, lr_abt
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_abt
    str r3, [sp, #-4]
    sub sp, sp, #4
.endm

.macro store_irq
    mrs r3, spsr_irq
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, lr_irq
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_irq
    str r3, [sp, #-4]
    sub sp, sp, #4
.endm

.macro store_fiq
    mrs r3, spsr_fiq
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, lr_fiq
    str r3, [sp, #-4]
    sub sp, sp, #4
    mrs r3, sp_fiq
    str r3, [sp, #-4]
    sub sp, sp, #4
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
    store_irq
    store_fiq
    store_abt
    store_svc
    store_usr
    bl undefined_instruction_interrupt
    load_all
    subs pc, lr, #0

software_interrupt:
    store_all
    store_und
    store_irq
    store_fiq
    store_abt
    store_usr
    mov r2, sp
    bl software_interrupt_interrupt
    load_all
    subs pc, lr, #0

prefetch_abort:
    store_all
    store_und
    store_irq
    store_fiq
    store_svc
    store_usr
    mov r2, sp
    bl prefetch_abort_interrupt
    load_all
    subs pc, lr, #0

data_abort:
    store_all
    store_und
    store_irq
    store_fiq
    store_svc
    store_usr
    mov r2, sp
    bl data_abort_interrupt
    load_all
    subs pc, lr, #4

irq:
    store_all
    store_und
    store_fiq
    store_abt
    store_svc
    store_usr
    bl irq_interrupt
    load_all
    subs pc, lr, #4

fiq:
    store_all
    store_und
    store_irq
    store_abt
    store_svc
    store_usr
    bl fiq_interrupt
    load_all
    subs pc, lr, #4

not_used:
