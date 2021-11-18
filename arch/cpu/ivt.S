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
    add sp, sp, #8
    ldmfd sp!, {r0-r12}
    ldr lr, [sp, #4]
    add sp, sp, #12
.endm

.macro save_und
    mrs r0, sp_und
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, lr_und
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, spsr_und
    str r0, [sp, #-4]
    sub sp, sp, #4
.endm

.macro save_svc
    mrs r0, sp_svc
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, lr_svc
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, spsr_svc
    str r0, [sp, #-4]
    sub sp, sp, #4
.endm

.macro save_abt
    mrs r0, sp_abt
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, lr_abt
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, spsr_abt
    str r0, [sp, #-4]
    sub sp, sp, #4
.endm

.macro save_irq
    mrs r0, sp_irq
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, lr_irq
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, spsr_irq
    str r0, [sp, #-4]
    sub sp, sp, #4
.endm

.macro save_fiq
    mrs r0, sp_fiq
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, lr_fiq
    str r0, [sp, #-4]
    sub sp, sp, #4
    mrs r0, spsr_fiq
    str r0, [sp, #-4]
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
    subs pc, lr, #4

software_interrupt:
    subs pc, lr, #4

prefetch_abort:
    store_all
    bl prefetch_abort_interrupt
    load_all
    subs pc, lr, #4

data_abort:
    subs pc, lr, #8

irq:
    subs pc, lr, #8

fiq:
    subs pc, lr, #8

not_used:
