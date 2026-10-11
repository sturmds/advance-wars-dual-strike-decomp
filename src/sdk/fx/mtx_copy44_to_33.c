#include "sdk/fx.h"

/*
 * Hand-written ARM assembly in the SDK: C does not produce the ldm/stm
 * sequences. Copies the upper left 3x3 of a 4x4 matrix.
 */
asm void Matrix_Take33From44(MtxFx44 *src, MtxFx33 *dest)
{
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    add r0, r0, #4
    stmia r1!, {r2, r3, ip}
    bx lr
}
