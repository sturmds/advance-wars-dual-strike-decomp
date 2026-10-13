#include "sdk/fp_status.h"

/*
 * Same update as FpStatus_Update on the status word, but both arguments and the
 * result use a different bit layout. Bits 0-4 and 16-20 of an argument move to
 * bits 0-4 and 8-12 of the word. The old word goes back out in the argument
 * layout with bit 30 set.
 */
#ifdef __MWERKS__
asm u32 FpStatus_UpdatePacked(u32 clearMask, u32 xorBits)
{
    and r2, r0, #0x1f
    and r3, r0, #0x1f0000
    orr r0, r2, r3, lsr #8
    and r2, r1, #0x1f
    and r3, r1, #0x1f0000
    orr r1, r2, r3, lsr #8
    stmfd sp!, {r0, r1, r2, r12, lr}
    bl __fp_status
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, r12, lr}
    ldr r2, [r3]
    bic r12, r2, r0
    eor r12, r12, r1
    str r12, [r3]
    and r0, r2, #0x1f
    and r2, r2, #0x1f00
    orr r0, r0, r2, lsl #8
    orr r0, r0, #0x40000000
    bx lr
}
#else
static u32 ToStatusLayout(u32 bits)
{
    return (bits & 0x1f) | ((bits & 0x1f0000) >> 8);
}

u32 FpStatus_UpdatePacked(u32 clearMask, u32 xorBits)
{
    u32 *status = __fp_status();
    u32 old = *status;

    *status = (old & ~ToStatusLayout(clearMask)) ^ ToStatusLayout(xorBits);
    return (old & 0x1f) | ((old & 0x1f00) << 8) | 0x40000000;
}
#endif
