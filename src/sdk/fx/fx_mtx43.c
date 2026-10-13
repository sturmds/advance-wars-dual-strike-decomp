#include "sdk/fx.h"

#ifdef __MWERKS__
/*
 * Hand-written Thumb assembly in the SDK: no compiler build produces the
 * stmia sequences from C. Kept as asm in a .c file, which mwccarm
 * assembles itself.
 *
 * Writes the rotation about X:
 *   1    0    0
 *   0    cos  sin
 *   0   -sin  cos
 *   0    0    0
 */
#pragma thumb on
asm void MTX_RotX43_(MtxFx43 *mtx, fx32 sin, fx32 cos)
{
    str r1, [r0, #20]
    neg r1, r1
    str r1, [r0, #28]
    mov r1, #1
    lsl r1, r1, #12
    stmia r0!, {r1}
    mov r3, #0
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    str r1, [r0, #4]
    add r0, #12
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
#pragma thumb reset
#else
/* Writes the rotation about X shown above. -sin wraps as neg does: negating
   0x80000000 gives 0x80000000. */
void MTX_RotX43_(MtxFx43* mtx, fx32 sin, fx32 cos) {
  fx32 neg_sin = (fx32)(0u - (u32)sin);

  mtx->m[0][0] = FX32_ONE;
  mtx->m[0][1] = 0;
  mtx->m[0][2] = 0;
  mtx->m[1][0] = 0;
  mtx->m[1][1] = cos;
  mtx->m[1][2] = sin;
  mtx->m[2][0] = 0;
  mtx->m[2][1] = neg_sin;
  mtx->m[2][2] = cos;
  mtx->m[3][0] = 0;
  mtx->m[3][1] = 0;
  mtx->m[3][2] = 0;
}
#endif
