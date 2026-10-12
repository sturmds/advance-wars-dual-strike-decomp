#include "types.h"

extern u8 *func_02129748(void);

/*
 * Converts an unsigned 32-bit integer to a float. The value is normalised
 * with clz and the exponent is built by hand. The low 8 bits that do not
 * fit in the 24-bit mantissa are rounded by the rounding mode in the
 * soft-float status word (bits 22-23: 0 nearest even, 1 toward plus infinity,
 * 2 and 3 truncate, as the value is positive); an inexact result sets bit 4 of the
 * status word, or goes to the trap call in _fflt when bit 12 is set.
 * The branches into _fflt are dcd words because the assembler cannot branch
 * into another function.
 */
asm float _ffltu(u32 x)
{
    cmp r0, #0
    bxeq lr
    mov r3, #0x9e
    bmi normalised
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
normalised:
    ands r2, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    dcd 0x1a000032 // bne _fflt + 0x9c (inexact trap)
    orr r3, r3, #0x10
    str r3, [r1]
    tst r3, #0x800000
    bxne lr
    tst r3, #0x400000
    dcd 0x1a000024 // bne _fflt + 0x7c (round toward plus infinity)
    tst r2, #0x80
    bxeq lr
    ands r1, r2, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
}
