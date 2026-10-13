#include "types.h"

#include "sdk/fp_status.h"

/*
 * Converts a signed 32-bit integer to a float. The magnitude is normalised
 * with clz and the exponent is built by hand. The low 8 bits that do not
 * fit in the 24-bit mantissa are rounded by the rounding mode in the
 * soft-float status word (bits 22-23: 0 nearest even, 1 toward plus infinity,
 * 2 toward minus infinity, 3 truncate). An inexact result sets bit 4 of the
 * status word, or calls the trap handler through __fp_trap_f when bit 12
 * is set. The labels ffltu_round_up and ffltu_inexact_trap are only
 * reached from _ffltu's branches.
 */
#ifdef __MWERKS__
asm float _fflt(int x)
{
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    bxeq lr
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #0x9e
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [ip]
    tst r3, #0x800000
    bne round_down_or_truncate
    tst r3, #0x400000
    bne round_up
    tst r1, #0x80
    bxeq lr
    ands r3, r1, #0x7f
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
ffltu_round_up:
    add r0, r0, #1
    bx lr
round_down_or_truncate:
    tst r3, #0x400000
    addeq r0, r0, r2, lsr #31
    bx lr
round_up:
    cmp r2, #0
    addpl r0, r0, #1
    bx lr
ffltu_inexact_trap:
    stmfd sp!, {lr}
    ldr r1, =0x40000308
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    stmfd sp!, {lr}
    ldr r1, =0x40000208
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
}
#else
#include "sdk/soft_float.h"

float _fflt(int x) {
  u32 sign = (u32)x & FP_SIGN;
  u32 abs = sign ? 0u - (u32)x : (u32)x;
  int lead;
  u32 bits;

  if (abs == 0) {
    return FpFloat(0);
  }
  lead = FpCountLeadingZeros(abs);
  abs <<= lead;
  bits = sign | ((abs << 1) >> 9) | ((u32)(FP_BIAS + 31 - lead) << 23);
  if ((abs & 0xff) == 0) {
    return FpFloat(bits);
  }
  return FpRoundInexact(bits, FpCompareHalf(abs & 0xff, 0x80), FP_INEXACT,
                        FP_OP_INT_TO_FLOAT);
}
#endif
