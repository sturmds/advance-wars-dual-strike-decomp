#include "sdk/fx.h"

#ifdef __MWERKS__
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
#else
/* Copies row by row, each row read in full before it is written, as the
   assembly does; the last column is 0 in rows 0 to 2 and 1.0 in row 3. */
void Matrix_Expand43Into44(MtxFx43* src, MtxFx44* dest) {
  int row;

  for (row = 0; row < 4; row++) {
    fx32 x = src->m[row][0];
    fx32 y = src->m[row][1];
    fx32 z = src->m[row][2];

    dest->m[row][0] = x;
    dest->m[row][1] = y;
    dest->m[row][2] = z;
    dest->m[row][3] = row == 3 ? FX32_ONE : 0;
  }
}
#endif
