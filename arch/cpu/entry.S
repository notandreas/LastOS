.section .init

.global _start
_start:
    /* Erkennen ob Hypervisor Mode aktiv */
    mrs r0, cpsr
    and r0, r0, #0x1F
    mov r1, #0x1A	//Modebits = 0x1A = Hypervisor Mode
    cmp r0, r1		//Vergleichen und Statusbits setzen für bedingte Sprünge

    /* Wenn Hypemode erkannt: abschalten (sprung nach _exitHyper)
     * wenn nicht erkannt: dann weitermachen (weiterlaufen nach _bsprak)
     */
    beq _exitHyper

    /* Qemu startet immer alle 4 Kerne
     * Wir lassen alle anderen Kerne endless loopen
     */
_checkCores:
    /* Id des Cpu Cores Abfragen */
    mrc p15, 0, r0, c0, c0, 5
    /* Falls nicht core 0 Core disablen */
    tst r0, #3
    bne _parkCore

/* not modeled in qemu 6.0 */
_enableAlignCheck:
    /* SCTLR des cp15 laden */
    mrc p15, 0, r0, c1, c0, 0
    /* A-bit (c1[1]) auf 1 setzen */
    orr r0, r0, #0x2
    /* neues SCTLR speichern */
    mcr p15, 0, r0, c1, c0, 0


_bsprak:
    /* Set IVT in VBAR */
    ldr r0, =_ivt
    mcr p15, 0, r0, c12, c0, 0
    /* Validen Stack Pointer setzten */
    @ldr sp,=0x00320000

    mrs r0, cpsr           @ save cpsr

    cps 0x1f               @ swap to sys
    ldr sp, =0x00015000    @ sys and usr sp
    cps 0x11               @ swap to fiq
    ldr sp, =0x0000D000    @ fiq sp
    cps 0x12               @ swap to irq
    ldr sp, =0x0000C000    @ irq sp
    cps 0x17               @ swap to abt
    ldr sp, =0x0000B000    @ abt sp
    cps 0x1b               @ swap to undef
    ldr sp, =0x0000A000    @ undef sp
    cps 0x13               @ swap to svc
    ldr sp, =0x00009000    @ svc sp

    msr cpsr_cxsf, r0      @ load cpsr again

    @@@@ möglicher bug mit zum ende hin zum usr switchen!!!
    cpsie if

    /* Zu c Code springen */
    bl  start_kernel
.Lend:
    WFI
    b .Lend

_parkCore:
    /* Interrupts für Core 1 bis 3 ausschalten  */
    cpsid if
    /* In Endlosschleife springen */
    b .Lend

_exitHyper:

    /* Rücksprungadresse ins Hypervisor Rücksprungregister schreiben.
     * Dies ist das Label _bsprak für die normale Programmausfürung.
     */
    ldr lr, =_checkCores
    msr ELR_hyp, lr

    /* Der Hypervisor mode ist auch "nur" eine etwas kompliziertere Exception.
     * Also springen wir aus dieser zurück.
     * (Rücksprunge in Aufgabe 2 sind anders zu lösen!).
     * Dazu zunächst das CPSR auslesen und die Modebits zum Supervisor ändern.
     * Danach dies in das SPSR für den Rücksprung schreiben.
     */
    mrs r0, cpsr
    bic r0, r0, #0x1F
    orr r0, r0, #0x13	//Supervisormode
    msr spsr_hyp, r0
    eret

#undef __ASSEMBLY__
