#include "sdk/fx.h"

/*
 * Hand-written ARM assembly in the SDK: C does not produce the stm
 * sequence. Writes the identity matrix (1.0 is 0x1000).
 */
asm void MTX_Identity43(MtxFx43 *mtx)
{
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
