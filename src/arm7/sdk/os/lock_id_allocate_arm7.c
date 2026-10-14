#include "sdk/os_arm7.h"

#ifdef __MWERKS__
/*
 * ARM7 copy of OS_GetLockID: finds a free lock ID in the ARM7's two-word map
 * at 0x027fffb8 in shared main RAM (bit set = free, top bit first), clears
 * its bit and returns the ID (0x80 to 0xbf), or OS_LOCK_ID_ERROR when both
 * words are empty. The ARM7 has no clz, so each word is scanned bit by bit.
 */
asm int LockId_Allocate_Arm7(void)
{
    ldr r3, =0x027fffb8
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
scan_first:
    tst r1, r0
    bne scanned_first
    add r2, r2, #1
    cmp r2, #32
    beq scanned_first
    mov r0, r0, lsr #1
    b scan_first
scanned_first:
    cmp r2, #32
    movne r0, #0x80
    bne found
    add r3, r3, #4
    ldr r1, [r3]
    mov r2, #0
    mov r0, #0x80000000
scan_second:
    tst r1, r0
    bne scanned_second
    add r2, r2, #1
    cmp r2, #32
    beq scanned_second
    mov r0, r0, lsr #1
    b scan_second
scanned_second:
    cmp r2, #32
    ldr r0, =OS_LOCK_ID_ERROR
    bxeq lr
    mov r0, #0xa0
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
int LockId_Allocate_Arm7(void) {
  vu32* map = OS7_LOCK_ID_FREE_MAP;
  int word;
  int bit;

  for (word = 0; word < LOCK_ID_MAP_WORDS; word++) {
    u32 free = map[word];

    for (bit = 0; bit < sizeof(u32) * BITS_PER_BYTE; bit++) {
      if (free & (0x80000000 >> bit)) {
        map[word] &= ~(0x80000000 >> bit);
        return 0x80 + word * 0x20 + bit;
      }
    }
  }
  return OS_LOCK_ID_ERROR;
}
#endif
