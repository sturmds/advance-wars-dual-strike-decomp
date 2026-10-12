#include "types.h"

extern u8 *func_02129748(void);
extern float func_02121ef4(float a, u32 op);

/*
 * Out-of-range tail of _ffix, entered by branch from it (it has no caller of
 * its own). It sets the invalid flag, or calls the trap handler through
 * func_02121ef4 when the invalid trap is enabled (status bit 8), and returns
 * 0x7fffffff or 0x80000000 by the sign of the input still in r0. The entry at
 * +0x24 skips the flag, for an input of exactly -2^31. The entry at +0x44 is
 * the inexact trap call.
 */
asm int func_02128438(float x)
{
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
saturate:
    mvn r0, r0, asr #31
    add r0, r0, #0x80000000
    bx lr
invalid_trap:
    ldr r1, =0x04c00048
    stmfd sp!, {lr}
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    ldr r1, =0x40c00048
    stmfd sp!, {lr}
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
}
