.macro save_all
    str sp, [sp, #-4]
    sub sp, sp, #4
    stmfd sp!, {r0-r12, lr, pc}

    mrs r0, cpsr
    str r0, [sp, #-4]
    sub sp, sp, #4
.endm

.macro load_all
    ldr r0, [sp, #4]
    add sp, sp, #4
    mrs cpsr_cxsf, r0

    ldmfd sp!, {r0-r12, lr, pc}
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
    ldr pc, _undefined_instruction
    ldr pc, _software_interrupt
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _not_used
    ldr pc, _irq
    ldr pc, _fiq

_undefined_instruction:     .word undefined_instruction
_software_interrupt:        .word software_interrupt
_prefetch_abort:            .word prefetch_abort
_data_abort:                .word data_abort
_not_used:                  .word not_used
_irq:                       .word irq
_fiq:                       .word fiq

reset:

undefined_instruction:
    subs pc, lr, #4

software_interrupt:
    subs pc, lr, #4

prefetch_abort:
    sub sp, sp, #4
    bl irq_interrupt
    ldr lr, [sp]
    add sp, sp, #4
    subs pc, lr, #4

data_abort:
    subs pc, lr, #8

irq:
    subs pc, lr, #8

fiq:
    subs pc, lr, #8

not_used:
