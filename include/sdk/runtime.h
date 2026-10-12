#ifndef SDK_RUNTIME_H
#define SDK_RUNTIME_H

#include "types.h"

/*
 * Integer division helpers of the Metrowerks ARM runtime. The compiler calls
 * them for `/` and `%` on 64-bit values and on 32-bit values it cannot divide
 * inline. A divisor of 0 returns the dividend unchanged in the 64-bit helpers.
 */
u64 _ll_udiv(u64 dividend, u64 divisor);
u64 _ull_mod(u64 dividend, u64 divisor);
s64 _ll_sdiv(s64 dividend, s64 divisor);
s64 _ll_mod(s64 dividend, s64 divisor);

/* Return the quotient in r0 and the remainder in r1. */
s32 _s32_div_f(s32 dividend, s32 divisor);
u32 _u32_div_f(u32 dividend, u32 divisor);
/* _u32_div_f after its check for a divisor of 0. */
u32 _u32_div_not_0_f(u32 dividend, u32 divisor);

/*
 * Shared tails of the 64-bit helpers. They expect the register state their
 * callers set up (r4 holds the flags and the saved registers are on the
 * stack), so C must not call them.
 */
asm void _ll_udiv_common(void);
asm void _ll_sdiv_common(void);
asm void _ll_div_core(void);

#endif
