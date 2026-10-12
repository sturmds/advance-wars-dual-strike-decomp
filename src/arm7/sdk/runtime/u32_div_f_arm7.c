#include "types.h"
#include "sdk/runtime.h"

/* One step of the unrolled division, as in _s32_div_f. */
#define DIV_STEP adcs r3, r1, r3, lsl #1; subcc r3, r3, r1; adcs r0, r0, r0;
#define DIV_STEP_4 DIV_STEP DIV_STEP DIV_STEP DIV_STEP

/*
 * ARM7 copy of src/sdk/runtime/u32_div_f.c. It keeps the plain name
 * _u32_div_f because the compiler calls it by that name for `/` in ARM7 C.
 * Unsigned 32-bit division: quotient in r0, remainder in r1. A divisor of 0
 * returns the dividend as the quotient and 0 as the remainder. The check
 * runs into _u32_div_not_0_f_Arm7, so the two must stay in this order.
 */
asm u32 _u32_div_f(u32 dividend, u32 divisor)
{
    cmp r1, #0
    bxeq lr
}

asm u32 _u32_div_not_0_f_Arm7(u32 dividend, u32 divisor)
{
    cmp r0, r1
    movcc r1, r0
    movcc r0, #0
    bxcc lr

    /* Count the leading quotient bits that must be 0, in steps of 4, and skip as many steps. */
    mov r2, #28
    mov r3, r0, lsr #4
    cmp r1, r3, lsr #12
    suble r2, r2, #16
    movle r3, r3, lsr #16
    cmp r1, r3, lsr #4
    suble r2, r2, #8
    movle r3, r3, lsr #8
    cmp r1, r3
    suble r2, r2, #4
    movle r3, r3, lsr #4
    mov r0, r0, lsl r2
    rsb r1, r1, #0
    adds r0, r0, r0
    add r2, r2, r2, lsl #1
    add pc, pc, r2, lsl #2
    nop
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    DIV_STEP_4
    mov r1, r3
    bx lr
}
