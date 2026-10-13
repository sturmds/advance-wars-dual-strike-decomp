#include "sdk/os.h"

#ifdef __MWERKS__
/*
 * OS_GetLockID: finds a free lock ID in the two-word map in shared
 * main RAM (see LOCK_ID_AVAILABLE_BITS) with clz, clears its bit and returns
 * the ID (0x40 to 0x7f), or OS_LOCK_ID_ERROR when both words are empty.
 */
asm int OS_GetLockID(void)
{
    ldr r3, =0x027fffb0
    ldr r1, [r3]
    clz r2, r1
    cmp r2, #32
    movne r0, #0x40
    bne found
    add r3, r3, #4
    ldr r1, [r3]
    clz r2, r1
    cmp r2, #32
    ldr r0, =OS_LOCK_ID_ERROR
    bxeq lr
    mov r0, #0x60
found:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3]
    bic r2, r2, r1
    str r2, [r3]
    bx lr
}
#else
/* The number of zero bits above the highest set bit; 32 for 0, as clz. */
static int CountLeadingZeros(u32 value) {
  int zeros = 0;

  while (zeros < 32 && (value & (0x80000000u >> zeros)) == 0) {
    zeros++;
  }
  return zeros;
}

/* Reads the chosen word a second time before clearing the bit, as the
   assembly does. Like the assembly, it does not stop interrupts. */
int OS_GetLockID(void) {
  volatile u32* bits = LOCK_ID_AVAILABLE_BITS;
  int firstID = 0x40;
  int zeros = CountLeadingZeros(*bits);

  if (zeros == 32) {
    bits++;
    zeros = CountLeadingZeros(*bits);
    if (zeros == 32) {
      return OS_LOCK_ID_ERROR;
    }
    firstID = 0x60;
  }
  *bits = *bits & ~(0x80000000u >> zeros);
  return firstID + zeros;
}
#endif
