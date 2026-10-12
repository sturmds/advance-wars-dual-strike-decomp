#include "types.h"

extern u8 *func_02129748(void);
extern double func_02121ea8(double a, u32 op);
extern float func_02121ef4(float a, u32 op);

/*
 * Converts a double (r0 low word, r1 high word) to a signed 32-bit integer,
 * rounding toward zero. Same status word and trap scheme as _ffix: an
 * inexact result sets bit 4 or calls the trap handler when bit 12 is set;
 * an out-of-range value returns 0x7fffffff or 0x80000000 and sets the
 * invalid flag (bit 0) or calls the trap handler when bit 8 is set.
 */
asm int _dfix(double x)
{
    bic r3, r1, #0x80000000
    ldr r2, =0x41e
    subs r2, r2, r3, lsr #20
    ble out_of_range
    cmp r2, #32
    bge below_one
    mov r3, r1, lsl #11
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    mov ip, r0, lsl #11
    cmp r1, #0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0
    rsb r2, r2, #32
    orrs ip, ip, r3, lsl r2
    bxeq lr
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
below_one:
    orrs r0, r0, r1, lsl #1
    bxeq lr
    mov r0, #0
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
out_of_range:
    ldr r2, =0xc1e00000
    cmp r1, r2
    beq maybe_min_int
invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
    mvn r0, r1, asr #31
    add r0, r0, #0x80000000
    bx lr
maybe_min_int:
    cmp r0, #0x200000
    bcs invalid
    cmp r0, #0
    mov r0, #0x80000000
    bxeq lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r2]
    bx lr
invalid_trap:
    ldr r2, =0x04c000c8
    stmfd sp!, {lr}
    bl func_02121ea8
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    ldr r1, =0x40c000c8
    stmfd sp!, {lr}
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
}
