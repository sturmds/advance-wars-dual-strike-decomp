#include "types.h"

/*
 * Converts an unsigned 32-bit integer to a double (result in r0 low, r1
 * high). Always exact, so it sets no status flags. The value is shifted
 * up with clz until its top bit is set; that bit becomes the hidden bit and
 * the exponent is 0x41e minus the shift.
 */
asm double _dfltu(u32 x)
{
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #30
    bmi normalised
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
normalised:
    mov r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
