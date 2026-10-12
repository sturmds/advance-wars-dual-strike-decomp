#include "types.h"

/*
 * Metrowerks runtime, hand-written ARM assembly: IEEE double subtraction
 * a - b for the soft-float ABI (a in r0:r1, b in r2:r3, result in r0:r1).
 *
 * Same layout as _dadd (see _dadd.c for the status word): operands of
 * opposite sign go to __dadd_same_sign with b negated, same signs fall
 * through into __dsub_same_sign, which _dadd also branches into.
 */

u8 *func_02129748(void);
double func_02121e58(double a, u32 op);
double func_02121db0(double a, double b, u32 op);
asm void __dadd_same_sign(void);

asm double _dsub(double a, double b)
{
    stmfd sp!, {r0, r1, r2, r3, r4, lr}
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    bmi __dadd_same_sign
}

/*
 * Entered from _dsub or _dadd with r0-r4 and lr pushed and both operands of
 * the same sign. Not callable from C.
 */
asm void __dsub_same_sign(void)
{
    /* Swap so that |a| >= |b|; the result then takes the other sign. */
    subs ip, r0, r2
    sbcs lr, r1, r3
    bcs unpack
    eor lr, lr, #0x80000000
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
unpack:
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
    beq same_exponent
    cmp r4, #32
    ble align_short
    cmp r4, #56
    movge r4, #63
    sub r4, r4, #32
    rsb lr, r4, #32
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi pack
    b normalize
align_short:
    rsb lr, r4, #32
    movs lr, r2, lsl lr
    rsb lr, r4, #32
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl normalize
pack:
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
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
toward_plus:
    cmp r1, #0
    addmi sp, sp, #16
    ldmmifd sp!, {r4, lr}
    bxmi lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, lr}
    bxne lr
    adds r0, r0, r1, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
same_exponent:
    /* Equal exponents: the difference is exact, it only needs normalizing. */
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq return_zero
    mov lr, ip, lsl #20
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0
    bmi exact_no_shift
    bne exact_normalize
    sub ip, ip, #32
    movs r1, r0
    mov r0, #0
    bmi exact_normalized
exact_normalize:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #32
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #32
    mov r0, r0, lsl r4
    sub ip, ip, r4
exact_normalized:
    cmp ip, #0
    bgt exact_pack
    rsb ip, ip, #12
    cmp ip, #32
    movge r0, r1
    movge r1, #0
    subge ip, ip, #32
    rsb r4, ip, #32
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
exact_no_shift:
    cmp r1, #0
    subges ip, ip, #1
exact_pack:
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
normalize:
    mov lr, ip, lsl #20
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0
    bne normalize_clz
    sub ip, ip, #32
    movs r1, r0
    mov r0, #0
    bmi normalized
normalize_clz:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #32
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #32
    mov r0, r0, lsl r4
    sub ip, ip, r4
normalized:
    cmp ip, #0
    orrgt ip, ip, lr, lsr #20
    bgt pack
    rsb ip, ip, #12
    cmp ip, #32
    movge r0, r1
    movge r1, #0
    subge ip, ip, #32
    rsb r4, ip, #32
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
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
    beq signal_invalid
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
    beq a_zero_b_subnormal
    cmn r4, #0x200000
    bne return_b
    orrs r4, r0, r1, lsl #1
    bne return_nan
    b return_inf
a_zero_b_subnormal:
    orrs r4, r0, r1, lsl #1
    beq return_zero
    b return_b
b_subnormal:
    orrs r4, r2, r3, lsl #1
    beq return_a_subnormal
    mov r4, #1
    bic r3, r3, #0x80000000
    orr ip, ip, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b align
return_b:
    mov r1, r3
    mov r0, r2
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
return_a_subnormal:
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
    beq signal_invalid
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
signal_invalid:
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
return_zero:
    /* x - x is +0, or -0 when rounding toward -inf. */
    mov r1, #0
    mov r0, #0
    stmfd sp!, {r0, r1, r2, ip, lr}
    bl func_02129748
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, ip, lr}
    ldr r3, [r3]
    tst r3, #0x800000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, lr}
    bxne lr
    mov r1, #0x80000000
    add sp, sp, #16
    ldmfd sp!, {r4, lr}
    bx lr
trap_invalid:
    ldmfd sp!, {r0, r1, r2, r3}
    ldr ip, =0x04000092
    stmfd sp!, {ip}
    bl func_02121db0
    add sp, sp, #4
    ldmfd sp!, {r4, lr}
    bx lr
trap_inexact:
    add sp, sp, #16
    ldr r2, =0x40000092
    bl func_02121e58
    ldmfd sp!, {r4, lr}
    bx lr
}
