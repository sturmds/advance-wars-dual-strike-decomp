#include "types.h"

/*
 * Converts a signed 32-bit integer to a double (result in r0 low, r1 high).
 * Always exact. The sign is kept in r2, the magnitude is shifted up with clz
 * and the exponent is 0x41e minus the shift.
 */
asm double _dflt(int x)
{
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #30
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    movs r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
