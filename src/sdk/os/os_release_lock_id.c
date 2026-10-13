#include "sdk/os.h"

#ifdef __MWERKS__
/*
 * OS_ReleaseLockID: gives a lock ID back. IDs 0x40 to 0x7f are tracked in two
 * words in shared main RAM (see LOCK_ID_AVAILABLE_BITS); a set bit means the ID
 * is free, and ID 0x40 is the top bit of the first word. OS_GetLockID clears
 * a bit to hand out an ID. Hand-written assembly in the original, like
 * OS_GetLockID.
 */
asm void OS_ReleaseLockID(int lockID)
{
    ldr r3, =0x027fffb0
    cmp r0, #0x60
    addpl r3, r3, #4
    subpl r0, r0, #0x60
    submi r0, r0, #0x40
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
}
#else
/* Follows the assembly for every lockID, not only 0x40 to 0x7f: the word
   is picked by the sign of lockID - 0x60, the shift uses the low byte of
   the bit number, and a shift of 32 or more sets no bit. */
void OS_ReleaseLockID(int lockID) {
  volatile u32* bits = LOCK_ID_AVAILABLE_BITS;
  u32 bit = (u32)lockID - 0x60u;
  u32 shift;
  u32 mask = 0;

  if ((bit & 0x80000000u) == 0) {
    bits++;
  } else {
    bit = (u32)lockID - 0x40u;
  }
  shift = bit & 0xff;
  if (shift < 32) {
    mask = 0x80000000u >> shift;
  }
  *bits = *bits | mask;
}
#endif
