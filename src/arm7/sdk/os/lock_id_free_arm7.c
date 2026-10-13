#include "sdk/os_arm7.h"

#ifdef __MWERKS__
/*
 * Gives back a lock ID from LockId_Allocate_Arm7: sets its bit (IDs 0x80
 * to 0xbf) in the free map at 0x027fffb8.
 */
asm void LockId_Free_Arm7(u16 lockID)
{
    ldr r3, =0x27fffb8
    cmp r0, #0xa0
    addpl r3, r3, #4
    subpl r0, r0, #0xa0
    submi r0, r0, #0x80
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
}
#else
/* For an ID outside 0x80 to 0xbf the assembly shifts by the low byte of
   (lockID - 0x80) or (lockID - 0xa0), and a shift of 32 or more gives 0, so
   IDs below 0x80 change nothing. The C does the same. */
void LockId_Free_Arm7(u16 lockID) {
  vu32* map = OS7_LOCK_ID_FREE_MAP;
  u32 shift;

  if (lockID >= 0xa0) {
    map++;
    shift = (lockID - 0xa0) & 0xff;
  } else {
    shift = (lockID - 0x80) & 0xff;
  }
  *map |= shift < 32 ? 0x80000000 >> shift : 0;
}
#endif
