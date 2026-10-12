#include "types.h"

/*
 * Metrowerks runtime, hand-written ARM assembly: IEEE double multiplication
 * for the soft-float ABI (a in r0:r1, b in r2:r3, product in r0:r1).
 * Status word as in _dadd.c, plus bit 3 underflow and bit 11 its trap
 * enable.
 *
 * _drsb (b - a) shares this file because it sits right after _dmul's
 * literal pool and falls through into _dsub, the next object. dsd cannot
 * give it a function symbol of its own (it refuses a function that starts
 * with an eor), so symbols.txt lists it as a label.
 */

u32 *__fp_status(void);
double __fp_trap_d(double a, u32 op);
double __fp_trap_dd(double a, double b, u32 op);

asm double _dmul(double a, double b)
{
    /* lr keeps the sign of the product; ip and r4 the exponents. */
    stmfd sp!, {r0, r1, r2, r3, r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov ip, r1, lsr #20
    mov r1, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r6, ip, lsl #21
    cmnne r6, #0x200000
    beq a_not_normal
    orr r1, r1, #0x80000000
    bic ip, ip, #0x800
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq b_not_normal
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
multiply:
    /* 64x64-bit product of the mantissas; the low word folds into a sticky bit. */
    add ip, r4, ip
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt normalized
    sub ip, ip, #1
    adds r0, r0, r0
    adc r1, r1, r1
normalized:
    add ip, ip, #2
    subs ip, ip, #0x400
    bmi underflow
    beq underflow
    mov r6, ip, lsl #20
    cmn r6, #0x100000
    bmi overflow
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #16
    str r3, [ip]
    tst r3, #0x800000
    bne toward_minus_or_zero
    tst r3, #0x400000
    bne toward_plus
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
round_up:
    adds r0, r0, #1
    adc r1, r1, #0
    mov r2, r1, lsl #1
    cmn r2, #0x200000
    addcc sp, sp, #16
    ldmccfd sp!, {r4, r5, r6, r7, lr}
    bxcc lr
    tst r3, #0x400
    bne trap_round_overflow
    orr r3, r3, #20
    str r3, [ip]
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
toward_plus:
    cmp lr, #0
    beq round_up
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    cmp lr, #0
    bne round_up
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
a_not_normal:
    bics ip, ip, #0x800
    beq a_subnormal
    orrs r6, r0, r1, lsl #1
    bne a_is_nan
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq a_inf_b_small
    cmn r5, #0x200000
    bne return_inf
    orrs r5, r2, r3, lsl #1
    beq return_inf
    b b_is_nan
a_inf_b_small:
    orrs r5, r3, r2
    beq signal_invalid
    b return_inf
b_not_normal:
    bics r4, r4, #0x800
    beq b_subnormal
    orrs r6, r2, r3, lsl #1
    bne b_is_nan
    b return_inf
a_subnormal:
    orrs r6, r0, r1, lsl #1
    beq a_zero
    mov ip, #1
    cmp r1, #0
    bne a_normalize_clz
    sub ip, ip, #32
    movs r1, r0
    mov r0, #0
    bmi a_normalized
a_normalize_clz:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #32
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #32
    mov r0, r0, lsl r6
    sub ip, ip, r6
a_normalized:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq b_not_normal
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b multiply
a_zero:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq return_zero
    cmn r5, #0x200000
    bne return_zero
    orrs r6, r2, r3, lsl #1
    beq signal_invalid
    b b_is_nan
b_subnormal:
    orrs r5, r2, r3, lsl #1
    beq return_zero
    mov r4, #1
    cmp r3, #0
    bne b_normalize_clz
    sub r4, r4, #32
    movs r3, r2
    mov r2, #0
    bmi multiply
b_normalize_clz:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #32
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #32
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b multiply
underflow:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r2, [r2]
    tst r2, #0x800
    bne trap_underflow
    cmn ip, #52
    beq underflow_half
    bmi underflow_to_zero
    mov r2, r1
    mov r3, r0
    add r4, ip, #52
    cmp r4, #32
    movge r2, r3
    movge r3, #0
    subge r4, r4, #32
    rsb r5, r4, #32
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb ip, ip, #12
    cmp ip, #32
    movge r0, r1
    movge r1, #0
    subge ip, ip, #32
    rsb r4, ip, #32
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    cmp r2, #0
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    bne tiny_toward_plus
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
tiny_toward_plus:
    cmp lr, #0
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
tiny_toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, r1, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
underflow_half:
    orr r0, r0, r1, lsl #1
    b round_tiny
return_inf:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
a_is_nan:
    tst r1, #0x40000000
    beq signal_invalid
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq return_default_nan
    cmn r5, #0x200000
    bne return_default_nan
    orrs r5, r2, r3, lsl #1
    beq return_default_nan
b_is_nan:
    mov r1, r3
    mov r3, r1
    mvn r0, #0
    bic r1, r0, #0x80000000
    tst r3, #0x40000000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne trap_invalid
    orr r3, r3, #1
    str r3, [r2]
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
signal_invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne trap_invalid
    orr r3, r3, #1
    str r3, [r2]
return_default_nan:
    mvn r0, #0
    bic r1, r0, #0x80000000
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
round_tiny:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    bne tiny_toward_plus
    cmp r2, #0
    addne r0, r0, #1
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
underflow_to_zero:
    mov r1, lr
    mov r0, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    cmp r1, #0
    addeq r0, r0, #1
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
overflow:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x400
    bne trap_overflow
    orr r3, r3, #20
    str r3, [r2]
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    tst r3, #0x800000
    bne overflow_toward_minus
    tst r3, #0x400000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    subs r0, r0, lr, lsr #31
    sbc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
overflow_toward_minus:
    subs r0, r0, #1
    sbc r1, r1, #0
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, lr, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
return_zero:
    mov r1, lr
    mov r0, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_round_overflow:
    add sp, sp, #16
    sub r1, r1, #0x60000000
    ldr r2, =0x10000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_overflow:
    add sp, sp, #16
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    sub ip, ip, #0x600
    orr r1, r1, ip, lsl #20
    ldr r2, =0x10000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_underflow:
    add sp, sp, #16
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    add ip, ip, #0x600
    orr r1, r1, ip, lsl #20
    ldr r2, =0x20000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_invalid:
    ldmfd sp!, {r0, r1, r2, r3}
    ldr ip, =0x04000093
    stmfd sp!, {ip}
    bl __fp_trap_dd
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_inexact:
    add sp, sp, #16
    ldr r2, =0x40000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}

/* Swaps a and b with three eors each, then runs on into _dsub. */
asm double _drsb(double a, double b)
{
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}
