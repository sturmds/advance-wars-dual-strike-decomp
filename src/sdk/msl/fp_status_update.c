#include "sdk/fp_status.h"

/*
 * Read-modify-write of the status word __fp_status returns: clears the bits
 * in clearMask, xors in xorBits, and returns the old word. The caller is the
 * soft-float trap code. Assembly: both arguments survive the call by being
 * saved and restored around it.
 */
#ifdef __MWERKS__
asm u32 FpStatus_Update(u32 clearMask, u32 xorBits)
{
    stmfd sp!, {r0, r1, r2, r12, lr}
    bl __fp_status
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, r12, lr}
    ldr r2, [r3]
    bic r12, r2, r0
    eor r12, r12, r1
    str r12, [r3]
    mov r0, r2
    bx lr
}
#else
u32 FpStatus_Update(u32 clearMask, u32 xorBits)
{
    u32 *status = __fp_status();
    u32 old = *status;

    *status = (old & ~clearMask) ^ xorBits;
    return old;
}
#endif
