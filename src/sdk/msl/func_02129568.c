#include "types.h"

extern u8 *func_02129748(void);

/*
 * Same update as func_02129540 on the status word, but both arguments and the
 * result use a different bit layout. Bits 0-4 and 16-20 of an argument move to
 * bits 0-4 and 8-12 of the word. The old word goes back out in the argument
 * layout with bit 30 set.
 */
asm u32 func_02129568(u32 clearMask, u32 xorBits)
{
    and r2, r0, #0x1f
    and r3, r0, #0x1f0000
    orr r0, r2, r3, lsr #8
    and r2, r1, #0x1f
    and r3, r1, #0x1f0000
    orr r1, r2, r3, lsr #8
    stmfd sp!, {r0, r1, r2, r12, lr}
    bl func_02129748
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
