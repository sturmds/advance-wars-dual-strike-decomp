#include "types.h"

extern asm BOOL Isd_RestoreFiq(BOOL enable);

/*
 * Masks FIQ (CPSR bit 6). Returns whether FIQ was enabled before.
 * The second pair of instructions is an unused entry that unmasks it.
 */
asm BOOL Isd_DisableFiq(void)
{
    mov r0, #0
    b Isd_RestoreFiq
    mov r0, #1
    b Isd_RestoreFiq
}

/*
 * Unmasks FIQ when enable is nonzero, masks it otherwise. Returns whether it was enabled before.
 * Needed to match: the entry jumps through a literal to the instruction 8 bytes in.
 */
asm BOOL Isd_RestoreFiq(BOOL enable)
{
    ldr r1, =Isd_RestoreFiq + 8
    bx r1
    mrs r3, CPSR
    mov r1, r3
    and r1, r1, #0x40
    mov r2, #0x40
    tst r0, r0
    orreq r3, r3, r2
    bicne r3, r3, r2
    msr CPSR_fc, r3
    tst r1, r1
    moveq r0, #1
    movne r0, #0
    bx lr
}
