#include "types.h"

#ifdef __MWERKS__
extern asm BOOL Isd_RestoreIrq(BOOL enable);

/*
 * Masks IRQ (CPSR bit 7). Returns whether IRQ was enabled before.
 * The second pair of instructions is an unused entry that unmasks it.
 */
asm BOOL Isd_DisableIrq(void)
{
    mov r0, #0
    b Isd_RestoreIrq
    mov r0, #1
    b Isd_RestoreIrq
}

/*
 * Unmasks IRQ when enable is nonzero, masks it otherwise. Returns whether it was enabled before.
 * Needed to match: the entry jumps through a literal to the instruction 8 bytes in.
 */
asm BOOL Isd_RestoreIrq(BOOL enable)
{
    ldr r1, =Isd_RestoreIrq + 8
    bx r1
    mrs r3, CPSR
    mov r1, r3
    and r1, r1, #0x80
    mov r2, #0x80
    tst r0, r0
    orreq r3, r3, r2
    bicne r3, r3, r2
    msr CPSR_fc, r3
    tst r1, r1
    moveq r0, #1
    movne r0, #0
    bx lr
}
#else
#error "Isd_DisableIrq, Isd_RestoreIrq: masking IRQ in the CPSR (mrs, msr) has no portable C equivalent"
#endif
