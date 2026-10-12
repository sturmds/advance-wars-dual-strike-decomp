#include "sdk/os.h"

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
