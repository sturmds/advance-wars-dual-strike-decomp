#include "types.h"

/*
 * Metrowerks runtime, hand-written ARM assembly: IEEE double addition for
 * the soft-float ABI (a in r0:r1, b in r2:r3, sum in r0:r1).
 *
 * _dadd checks the signs only. Operands of opposite sign go to _dsub's
 * magnitude code with b negated; same signs fall through into
 * __dadd_same_sign. The two halves are separate functions so that _dsub can
 * branch into the second one by name.
 *
 * func_02129748 returns the floating-point status word: bit 0 invalid,
 * bit 2 overflow, bit 4 inexact; bits 8, 10 and 12 enable the trap for each;
 * bits 22-23 the rounding mode (0 nearest, 1 toward +inf, 2 toward -inf,
 * 3 toward zero). A trap goes to func_02121e58 or func_02121db0 with an
 * exception code.
 */

u8 *func_02129748(void);
double func_02121e58(double a, u32 op);
double func_02121db0(double a, double b, u32 op);
asm void __dsub_same_sign(void);

asm double _dadd(double a, double b)
{
    stmfd sp!, {r0, r1, r2, r3, r4, lr}
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    bmi __dsub_same_sign
}

/*
 * Entered from _dadd or _dsub with r0-r4 and lr pushed and both operands of
 * the same sign. Not callable from C.
 */
asm void __dadd_same_sign(void)
{
    /* Swap so that |a| >= |b|. */
    subs ip, r0, r2
    sbcs lr, r1, r3
    bcs unpack
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
unpack:
    /*
     * Mantissas move to the top of r1:r0 and r3:r2 with the implicit 1 in
     * bit 31; ip and r4 keep the sign and exponent.
     */
    mov lr, #0x80000000
    mov ip, r1, lsr #20
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r4, ip, lsl #21
    cmnne r4, #0x200000
    beq a_not_normal
    mov r4, r3, lsr #20
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs lr, r4, lsl #21
    beq b_not_normal
align:
    /* Shift b right by the exponent difference; bits shifted out stick in bit 0. */
    subs r4, ip, r4
    beq add_mantissas
    cmp r4, #32
    ble align_short
    cmp r4, #56
    movge r4, #63
    sub r4, r4, #32
    rsb lr, r4, #32
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    bcc pack
    b carry_out
align_short:
    rsb lr, r4, #32
    movs lr, r2, lsl lr
    rsb lr, r4, #32
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
add_mantissas:
    adds r0, r0, r2
    adcs r1, r1, r3
    bcc pack
carry_out:
    add ip, ip, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, ip, lsl #21
    cmn lr, #0x200000
    beq overflow
pack:
    /* r2 gets the 11 bits that do not fit; zero means the sum is exact. */
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, ip, lsl #20
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
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
    /* Round to nearest, ties to even. */
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
round_up:
    adds r0, r0, #1
    adc r1, r1, #0
    mov r2, r1, lsl #1
    cmn r2, #0x200000
    addcc sp, sp, #16
    ldmccfd sp!, {r4, lr}
    bxcc lr
    tst r3, #0x400
    bne trap_round_overflow
    orr r3, r3, #20
    str r3, [ip]
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
toward_plus:
    cmp r1, #0
    bpl round_up
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, lr}
    bxne lr
    cmp r1, #0
    bmi round_up
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
a_not_normal:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics ip, ip, #0x800
    beq a_subnormal
    orrs r4, r0, r1, lsl #1
    bne a_is_nan
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq return_inf
    cmn r4, #0x200000
    bne return_inf
    orrs r4, r2, r3, lsl #1
    beq return_inf
    b b_is_nan
b_not_normal:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic ip, ip, #0x800
    bics r4, r4, #0x800
    beq b_subnormal
    orrs r4, r2, r3, lsl #1
    bne b_is_nan
    b return_inf
a_subnormal:
    orrs r4, r0, r1, lsl #1
    beq a_zero
    mov ip, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    cmnne r4, #0x200000
    mov r4, r4, lsr #21
    orr r4, r4, lr, lsr #20
    beq b_not_normal
    orr r3, r3, #0x80000000
    orr ip, ip, lr, lsr #20
    b align
a_zero:
    mov ip, r3, lsr #20
    mov r1, r3, lsl #11
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #11
    movs r4, ip, lsl #21
    beq return_b
    cmn r4, #0x200000
    bne return_b
    orrs r4, r0, r1, lsl #1
    beq return_inf
    b return_nan
b_subnormal:
    orrs r4, r2, r3, lsl #1
    beq return_subnormal
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl add_subnormals
    orr ip, ip, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b align
add_subnormals:
    adds r0, r0, r2
    adcs r1, r1, r3
    bcc pack_subnormal
    add ip, ip, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
pack_subnormal:
    cmp r1, #0
    subges ip, ip, #1
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
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
    /* Round to nearest, ties to even. */
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
return_b:
    mov r1, r3
    mov r0, r2
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
return_subnormal:
    cmp r1, #0
    subges ip, ip, #1
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
return_inf:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
a_is_nan:
    tst r1, #0x40000000
    beq a_signaling_nan
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq return_default_nan
    cmn r4, #0x200000
    bne return_default_nan
    orrs r4, r2, r3, lsl #1
    beq return_default_nan
b_is_nan:
    mov r1, r3
return_nan:
    mov r3, r1
    mvn r0, #0
    bic r1, r0, #0x80000000
    tst r3, #0x40000000
    addne sp, sp, #16
    ldmnefd sp!, {r4, lr}
    bxne lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne trap_invalid
    orr r3, r3, #1
    str r3, [r2]
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
a_signaling_nan:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
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
    ldmfd sp!, {r4, lr}
    bx lr
overflow:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x400
    bne trap_overflow
    orr r3, r3, #20
    str r3, [ip]
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    tst r3, #0x800000
    bne overflow_toward_minus
    tst r3, #0x400000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    subs r0, r0, lr, lsr #31
    sbc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
overflow_toward_minus:
    subs r0, r0, #1
    sbc r1, r1, #0
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, lr}
    bxne lr
    adds r0, r0, lr, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
trap_round_overflow:
    add sp, sp, #16
    sub r1, r1, #0x60000000
    ldr r2, =0x10000091
    bl func_02121e58
    ldmfd sp!, {r4, lr}
    bx lr
trap_overflow:
    add sp, sp, #16
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    ldr r2, =0x000001ff
    orr r1, r1, r2, lsl #20
    orr r1, r1, lr
    ldr r2, =0x10000091
    bl func_02121e58
    ldmfd sp!, {r4, lr}
    bx lr
trap_invalid:
    ldmfd sp!, {r0, r1, r2, r3}
    ldr ip, =0x04000091
    stmfd sp!, {ip}
    bl func_02121db0
    add sp, sp, #4
    ldmfd sp!, {r4, lr}
    bx lr
trap_inexact:
    add sp, sp, #16
    ldr r2, =0x40000091
    bl func_02121e58
    ldmfd sp!, {r4, lr}
    bx lr
}
