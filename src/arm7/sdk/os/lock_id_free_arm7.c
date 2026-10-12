#include "sdk/os_arm7.h"

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
