#include "types.h"
#include "sdk/runtime.h"

s32 func_arm7_03806d4c(s32 dividend, s32 divisor);   /* ARM7 copy of _s32_div_f */

/*
 * ARM7 copy of src/sdk/runtime/ll_sdiv.c: signed 64-bit remainder and
 * division, and the 64-bit division loop that _ll_udiv_Arm7 shares. The
 * functions in this file run into each other, so they must stay in this
 * order.
 *
 * Both entries set flags in r4: bit 31 set means the result is negated at the
 * end, bit 0 set means the remainder is returned instead of the quotient.
 */

/* The remainder takes the sign of the dividend. */
asm s64 _ll_mod_Arm7(s64 dividend, s64 divisor)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    b _ll_sdiv_common_Arm7
}

/* The quotient is negative when the signs differ. */
asm s64 _ll_sdiv_Arm7(s64 dividend, s64 divisor)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
}

/* Dividend in r0:r1, divisor in r2:r3. */
asm void _ll_sdiv_common_Arm7(void)
{
    orrs r5, r3, r2
    bne @divisor_not_0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr

@divisor_not_0:
    /* Both values fit in 32 bits: one 32-bit division gives quotient and remainder. */
    mov r5, r0, lsr #31
    add r5, r5, r1
    mov r6, r2, lsr #31
    add r6, r6, r3
    orrs r6, r5, r6
    bne @divide_magnitudes
    mov r1, r2
    bl func_arm7_03806d4c
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #31
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr

@divide_magnitudes:
    cmp r1, #0
    bge @dividend_positive
    rsbs r0, r0, #0
    rsc r1, r1, #0
@dividend_positive:
    cmp r3, #0
    bge _ll_div_core_Arm7
    rsbs r2, r2, #0
    rsc r3, r3, #0
}

/*
 * Divides the unsigned dividend r0:r1 by the nonzero divisor r2:r3, one
 * quotient bit per step, without restoring the remainder after a step that
 * goes negative. r12:r11 collects the quotient, r7 extends the remainder, r6
 * counts the steps left, r5 is how far the divisor was shifted up.
 */
asm void _ll_div_core_Arm7(void)
{
    orrs r5, r1, r0
    beq @dividend_0
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi @shift_dividend
@shift_divisor:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl @shift_divisor
    add r6, r6, r5
@shift_dividend:
    cmp r1, #0
    blt @start
@shift_dividend_loop:
    cmp r6, #1
    beq @start
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl @shift_dividend_loop
@start:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b @subtract

@bit_1:
    orr r12, r12, #1
    subs r6, r6, #1
    beq @done
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
@subtract:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @bit_1
@bit_0:
    subs r6, r6, #1
    beq @fix_remainder
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @bit_1
    b @bit_0

@fix_remainder:
    adds r0, r0, r2
    adc r1, r1, r3
@done:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq @apply_sign
    /* Remainder: undo the shift the divisor had. */
    subs r7, r5, #32
    movge r0, r1, lsr r7
    bge @high_0
    rsb r7, r5, #32
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b @apply_sign
    /* Never reached. */
    mov r0, r1, lsr r7
    mov r1, #0
@apply_sign:
    cmp r4, #0
    blt @negate
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@negate:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr

@dividend_0:
    mov r0, #0
@high_0:
    mov r1, #0
    cmp r4, #0
    blt @negate
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}
