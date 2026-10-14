#include "sdk/fx.h"

#ifdef __MWERKS__
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
#else
/* Writes 1.0 on the diagonal of the 3x3 part and 0 everywhere else,
   translation row included. */
void MTX_Identity43(MtxFx43* mtx) {
  int row;
  int col;

  for (row = 0; row < ARRAY_COUNT(mtx->m); row++) {
    for (col = 0; col < ARRAY_COUNT(mtx->m[0]); col++) {
      mtx->m[row][col] = row == col ? FX32_ONE : 0;
    }
  }
}
#endif
