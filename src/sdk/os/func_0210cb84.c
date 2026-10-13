#include "types.h"

#ifdef __MWERKS__
/*
 * Hand-written SDK assembly. Copies 32 bytes from src + 32 to dst: the
 * second stmia does not advance dst, so it overwrites the first block.
 * Address taken in func_0210b5b8 and func_0210b5d4; purpose not known.
 */
asm void func_0210cb84(void *src, void *dst)
{
    stmfd sp!, {r4-r8}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmfd sp!, {r4-r8}
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Writes both blocks to dst, in the assembly's order: all eight loads of a
   block, then its eight stores in rising address order. dst may be a
   hardware register range, so the first block's stores are kept. Assumes
   both pointers are word-aligned; ldm and stm would round them down, so a
   misaligned one stops the program. */
void func_0210cb84(void* src, void* dst) {
  const u32* from = src;
  volatile u32* to = dst;
  u32 block[8];
  int pass;
  int i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dst % 4 != 0) {
    fprintf(stderr, "func_0210cb84: %p or %p is not word-aligned\n", src,
            dst);
    abort();
  }
  for (pass = 0; pass < 2; pass++) {
    for (i = 0; i < 8; i++) {
      block[i] = *from++;
    }
    for (i = 0; i < 8; i++) {
      to[i] = block[i];
    }
  }
}
#endif
