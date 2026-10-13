#include "sdk/fx.h"

#ifdef __MWERKS__
/*
 * Hand-written Thumb assembly in the SDK, like MTX_RotX43_: the stmia
 * sequences have no C form. Writes the rotation about Y:
 *   cos  0  -sin
 *   0    1   0
 *   sin  0   cos
 *   0    0   0
 */
#pragma thumb on
asm void MTX_RotY43_(MtxFx43 *mtx, fx32 sin, fx32 cos)
{
    str r1, [r0, #24]
    mov r3, #0
    stmia r0!, {r2, r3}
    neg r1, r1
    stmia r0!, {r1, r3}
    mov r1, #1
    lsl r1, r1, #12
    stmia r0!, {r1, r3}
    add r0, #4
    mov r1, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
#pragma thumb reset
#else
/* Writes the rotation about Y shown above. -sin wraps as neg does: negating
   0x80000000 gives 0x80000000. */
void MTX_RotY43_(MtxFx43* mtx, fx32 sin, fx32 cos) {
  fx32 neg_sin = (fx32)(0u - (u32)sin);

  mtx->m[0][0] = cos;
  mtx->m[0][1] = 0;
  mtx->m[0][2] = neg_sin;
  mtx->m[1][0] = 0;
  mtx->m[1][1] = FX32_ONE;
  mtx->m[1][2] = 0;
  mtx->m[2][0] = sin;
  mtx->m[2][1] = 0;
  mtx->m[2][2] = cos;
  mtx->m[3][0] = 0;
  mtx->m[3][1] = 0;
  mtx->m[3][2] = 0;
}
#endif
