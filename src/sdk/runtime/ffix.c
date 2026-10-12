#include "types.h"

extern u8 *func_02129748(void);

/*
 * Converts a float to a signed 32-bit integer, rounding toward zero.
 * func_02129748 returns the soft-float status word: flags in bits 0-4
 * (bit 4 inexact), trap enables in bits 8-12 (bit 12 inexact). An inexact
 * result sets the flag, or calls the trap handler when its enable is set.
 * Values of 2^31 and above, and negative 2^31 exactly, continue in
 * func_02128438 (the next function in the ROM).
 * The branches into func_02128438 are encoded with dcd because the
 * assembler cannot branch into another function.
 */
asm int _ffix(float x)
{
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #23
    ble out_of_range
    cmp r2, #32
    bge below_one
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    rsb r2, r2, #32
    movs r1, r1, lsl r2
    bxeq lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x1000
    dcd 0x1a000022 // bne func_02128438 + 0x44 (inexact trap)
    orr r3, r3, #0x10
    str r3, [r2]
    bx lr
below_one:
    movs r0, r1
    bxeq lr
    mov r0, #0
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    dcd 0x1a000015 // bne func_02128438 + 0x44 (inexact trap)
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
out_of_range:
    cmp r0, #0xcf000000
    dcd 0x0a000008 // beq func_02128438 + 0x24 (exactly -2^31)
}
