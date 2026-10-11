#include "sdk/fx.h"

/*
 * Hand-written Thumb assembly in the SDK, like MTX_RotX43_: the stmia
 * sequences have no C form. Writes the rotation about Z:
 *   cos  sin  0
 *  -sin  cos  0
 *   0    0    1
 *   0    0    0
 */
#pragma thumb on
asm void Matrix_SetZTurn43FromSinCos(MtxFx43 *mtx, fx32 sin, fx32 cos)
{
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2, r3}
    mov r1, #0
    mov r2, #0
    mov r3, #1
    lsl r3, r3, #12
    stmia r0!, {r1, r2, r3}
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    bx lr
}
#pragma thumb reset
