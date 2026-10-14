#include "sdk/fx.h"

#ifdef __MWERKS__
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
#else
/* Copies the first three columns of rows 0 to 2, each row read in full
   before it is written, as the assembly does. */
void Matrix_Take33From44(MtxFx44* src, MtxFx33* dest) {
  int row;

  for (row = 0; row < ARRAY_COUNT(dest->m); row++) {
    fx32 x = src->m[row][0];
    fx32 y = src->m[row][1];
    fx32 z = src->m[row][2];

    dest->m[row][0] = x;
    dest->m[row][1] = y;
    dest->m[row][2] = z;
  }
}
#endif
