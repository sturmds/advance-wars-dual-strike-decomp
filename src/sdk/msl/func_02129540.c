#include "types.h"

extern u8 *func_02129748(void);

/*
 * Read-modify-write of the status word func_02129748 returns: clears the bits
 * in r0, xors in r1, and returns the old word. The caller is the
 * soft-float trap code. Assembly: r0 and r1 survive the call by being saved
 * and restored around it.
 */
asm u32 func_02129540(u32 clearMask, u32 xorBits)
{
    stmfd sp!, {r0, r1, r2, r12, lr}
    bl func_02129748
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, r12, lr}
    ldr r2, [r3]
    bic r12, r2, r0
    eor r12, r12, r1
    str r12, [r3]
    mov r0, r2
    bx lr
}
