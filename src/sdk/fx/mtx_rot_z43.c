#include "sdk/fx.h"

#ifdef __MWERKS__
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
#else
/* Writes the rotation about Z shown above. -sin wraps as neg does: negating
   0x80000000 gives 0x80000000. */
void Matrix_SetZTurn43FromSinCos(MtxFx43* mtx, fx32 sin, fx32 cos) {
  fx32 neg_sin = (fx32)(0u - (u32)sin);

  mtx->m[0][0] = cos;
  mtx->m[0][1] = sin;
  mtx->m[0][2] = 0;
  mtx->m[1][0] = neg_sin;
  mtx->m[1][1] = cos;
  mtx->m[1][2] = 0;
  mtx->m[2][0] = 0;
  mtx->m[2][1] = 0;
  mtx->m[2][2] = FX32_ONE;
  mtx->m[3][0] = 0;
  mtx->m[3][1] = 0;
  mtx->m[3][2] = 0;
}
#endif
