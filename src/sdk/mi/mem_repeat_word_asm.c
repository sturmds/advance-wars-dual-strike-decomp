#include "types.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly: fills size bytes at dest with a 32-bit value.
 * The conditional stmia is not something the compiler emits from C.
 */
asm void Mem_RepeatWordAsm(u32 value, u32 *dest, u32 size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stores value into each word from dest while the word starts before
   dest + size, so a size that is not a multiple of 4 writes one more word.
   Assumes dest is word-aligned; the store would round it down, so a
   misaligned one stops the program. */
void Mem_RepeatWordAsm(u32 value, u32* dest, u32 size) {
  u32 i;

  if ((uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "Mem_RepeatWordAsm: %p is not word-aligned\n", (void*)dest);
    abort();
  }
  for (i = 0; i < size; i += 4) {
    *dest++ = value;
  }
}
#endif
