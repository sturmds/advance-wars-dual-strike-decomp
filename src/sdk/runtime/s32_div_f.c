#include "types.h"
#include "sdk/runtime.h"

#ifdef __MWERKS__
/*
 * One step of the unrolled division: shift the next dividend bit into the
 * remainder r3, try subtracting the divisor (r1 holds it negated), and shift
 * the result bit into the quotient r0.
 */
#define DIV_STEP adcs r3, r1, r3, lsl #1; subcc r3, r3, r1; adcs r0, r0, r0;
#define DIV_STEP_4 DIV_STEP DIV_STEP DIV_STEP DIV_STEP

/*
 * Signed 32-bit division: quotient in r0, remainder in r1. The quotient is
 * negative when the signs differ, the remainder takes the sign of the
 * dividend. A divisor of 0 returns the dividend as the quotient and 0 as the
 * remainder.
 */
asm s32 _s32_div_f(s32 dividend, s32 divisor)
{
    /* r12: bit 31 negates the quotient, bit 0 negates the remainder. */
    eor r12, r0, r1
    and r12, r12, #0x80000000
    cmp r0, #0
    rsblt r0, r0, #0
    addlt r12, r12, #1
    cmp r1, #0
    rsblt r1, r1, #0
    beq @apply_signs
    cmp r0, r1
    movcc r1, r0
    movcc r0, #0
    bcc @apply_signs

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

@apply_signs:
    ands r3, r12, #0x80000000
    rsbne r0, r0, #0
    ands r3, r12, #1
    rsbne r1, r1, #0
    bx lr
}
#else
/* Assumes the host's int is 32 bits wide and converts out-of-range
   unsigned values to signed by wrapping, as gcc and clang do. */
typedef char s32_is_32_bits[sizeof(s32) == 4 ? 1 : -1];

/* The quotient. The assembly also leaves the remainder in r1, which a C
   return value cannot carry; host code uses `%` for it. The assembly
   divides the magnitudes as unsigned values, so a divisor of 0 returns the
   dividend and INT_MIN / -1 returns INT_MIN; the C does the same. */
s32 _s32_div_f(s32 dividend, s32 divisor) {
  u32 magnitude = dividend < 0 ? 0u - (u32)dividend : (u32)dividend;
  u32 divisorMagnitude = divisor < 0 ? 0u - (u32)divisor : (u32)divisor;
  u32 quotient;

  if (divisor == 0) {
    return dividend;
  }
  quotient = magnitude / divisorMagnitude;
  if ((dividend < 0) != (divisor < 0)) {
    quotient = 0u - quotient;
  }
  return (s32)quotient;
}
#endif
