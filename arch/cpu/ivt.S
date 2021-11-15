.section .ivt
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

undefined_instruction:
    subs pc, lr, #4

software_interrupt:
    subs pc, lr, #4

prefetch_abort:
    subs pc, lr, #4

data_abort:
    subs pc, lr, #8

irq:
    subs pc, lr, #8

fiq:
    subs pc, lr, #8

reset:
not_used:
