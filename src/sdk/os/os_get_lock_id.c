#include "sdk/os.h"

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
