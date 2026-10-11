#include "sdk/fx.h"

/*
 * Hand-written ARM assembly in the SDK: C does not produce the ldm/stm
 * sequences. Copies the four rows of a 4x3 matrix into a 4x4 matrix with
 * the last column 0, 0, 0, 1.
 */
asm void Matrix_Expand43Into44(MtxFx43 *src, MtxFx44 *dest)
{
    stmfd sp!, {r4}
    mov ip, #0
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    mov ip, #0x1000
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, ip}
    ldmfd sp!, {r4}
    bx lr
}
