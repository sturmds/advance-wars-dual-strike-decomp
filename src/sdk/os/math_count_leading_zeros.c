#include "types.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly: the clz instruction has no C form in this
 * compiler.
 */
asm u32 MATH_CountLeadingZeros(u32 value)
{
    clz r0, r0
    bx lr
}
#else
/* The number of zero bits above the highest set bit; 32 for 0. */
u32 MATH_CountLeadingZeros(u32 value) {
  u32 zeros = 0;

  while (zeros < 32 && (value & (0x80000000u >> zeros)) == 0) {
    zeros++;
  }
  return zeros;
}
#endif
