#include "types.h"
#include "sdk/runtime.h"

#ifdef __MWERKS__
/* One step of the unrolled division, as in _s32_div_f. */
#define DIV_STEP adcs r3, r1, r3, lsl #1; subcc r3, r3, r1; adcs r0, r0, r0;
#define DIV_STEP_4 DIV_STEP DIV_STEP DIV_STEP DIV_STEP

/*
 * Unsigned 32-bit division: quotient in r0, remainder in r1. A divisor of 0
 * returns the dividend as the quotient and 0 as the remainder. The check
 * runs into _u32_div_not_0_f, so the two must stay in this order.
 */
asm u32 _u32_div_f(u32 dividend, u32 divisor)
{
    cmp r1, #0
    bxeq lr
}

asm u32 _u32_div_not_0_f(u32 dividend, u32 divisor)
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
#else
/* Assumes the host's unsigned int is 32 bits wide, like u32 on the DS. */
typedef char u32_is_32_bits[sizeof(u32) == 4 ? 1 : -1];

/* The quotient. The assembly also leaves the remainder in r1, which a C
   return value cannot carry; host code uses `%` for it. A divisor of 0
   returns the dividend, as the assembly does. */
u32 _u32_div_f(u32 dividend, u32 divisor) {
  if (divisor == 0) {
    return dividend;
  }
  return _u32_div_not_0_f(dividend, divisor);
}

/* The assembly does not check the divisor: with 0 it ends with a quotient of
   0 (and the dividend as the remainder), so the C returns 0 too. */
u32 _u32_div_not_0_f(u32 dividend, u32 divisor) {
  if (divisor == 0) {
    return 0;
  }
  return dividend / divisor;
}
#endif
