#include "types.h"

#include "sdk/fp_status.h"

extern void _fsub(void);

/*
 * Soft-float single-precision add: r0 = r0 + r1, as IEEE bit patterns.
 * Operands of different sign go to the subtract path in _fsub, with b negated.
 * The operands and the operation number (1 = add) stay on the stack for the
 * trap wrappers.
 *
 * __fp_status returns the floating-point status word. Bits used here:
 * 0x1 invalid, 0x4 overflow, 0x10 inexact (sticky flags); 0x100, 0x400,
 * 0x1000 enable the trap for each; bits 22-23 pick the rounding mode
 * (0 nearest, 1 toward +inf, 2 toward -inf, 3 toward zero).
 */
#ifdef __MWERKS__
asm void _fadd(void)
{
    mov r2, #1
    stmfd sp!, {r0, r1, r2}
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a00077d /* bmi _fsub+0x14, encoded by hand: the assembler takes no symbol offsets */
    subs ip, r0, r1
    subcc r0, r0, ip
    addcc r1, r1, ip
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq a_exp_zero_or_max
    mov ip, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, ip, #0xff
    beq b_exp_zero
align_b:
    subs ip, r3, ip
    beq add_mantissas
    cmp ip, #28
    movge ip, #31
    rsb r2, ip, #32
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #1
add_mantissas:
    adds r0, r0, r1
    bcc round_result
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq overflow
round_result:
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    addeq sp, sp, #12
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #16
    str r3, [ip]
    tst r3, #0x800000
    bne round_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne round_toward_plus_inf
    tst r1, #0x80
    addeq sp, sp, #12
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addeq sp, sp, #12
    bxeq lr
round_up:
    add r0, r0, #1
    mov r1, r0, lsl #1
    cmn r1, #0x1000000
    addcc sp, sp, #12
    bxcc lr
    tst r3, #0x400
    bne overflow_trap_after_round
    orr r3, r3, #20
    str r3, [ip]
    add sp, sp, #12
    bx lr
round_toward_plus_inf:
    cmp r0, #0
    bpl round_up
    add sp, sp, #12
    bx lr
round_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addne sp, sp, #12
    bxne lr
    cmp r0, #0
    bmi round_up
    add sp, sp, #12
    bx lr
a_exp_zero_or_max:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq a_zero_or_subnormal
    movs r0, r0, lsl #1
    bne a_nan
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq return_inf
    cmp ip, #0xff
    blt return_inf
    cmp r1, #0
    beq return_inf
    b b_nan
b_exp_zero:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq b_zero_or_subnormal
b_inf_or_nan:
    movs r1, r1, lsl #1
    bne b_nan
    b return_inf
a_zero_or_subnormal:
    movs r0, r0, lsl #1
    beq a_zero
    mov r3, #1
    mov r0, r0, lsr #1
    mov ip, r1, lsr #23
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq b_zero_or_subnormal
    cmp ip, #0xff
    beq b_inf_or_nan
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    b align_b
a_zero:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq return_b
    cmp r3, #0xff
    blt return_b
    cmp r0, #0
    beq return_inf
    b a_zero_b_nan
b_zero_or_subnormal:
    movs r1, r1, lsl #1
    beq return_a
    mov r1, r1, lsr #1
    mov ip, #1
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    cmp r0, #0
    bmi align_b
    adds r0, r0, r1
    bcc round_subnormal_sum
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add ip, ip, #1
round_subnormal_sum:
    cmp r0, #0
    subge ip, ip, #1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, ip, lsl #23
    addeq sp, sp, #12
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #16
    str r3, [ip]
    tst r3, #0x800000
    bne round_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne round_toward_plus_inf
    tst r1, #0x80
    addeq sp, sp, #12
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    add sp, sp, #12
    bx lr
return_b:
    mov r0, r1
    add sp, sp, #12
    bx lr
return_a:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    add sp, sp, #12
    bx lr
overflow:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x400
    bne overflow_trap
    orr r3, r3, #20
    str r3, [ip]
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    tst r3, #0x800000
    bne overflow_toward_minus_inf
    tst r3, #0x400000
    subne r0, r0, r2, lsr #31
    add sp, sp, #12
    bx lr
overflow_toward_minus_inf:
    sub r0, r0, #1
    tst r3, #0x400000
    addeq r0, r0, r2, lsr #31
    add sp, sp, #12
    bx lr
a_zero_b_nan:
    mov r3, r0
    mvn r0, #0x80000000
    tst r3, #0x80000000
    addne sp, sp, #12
    bxne lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    orr r3, r3, #1
    str r3, [r2]
    add sp, sp, #12
    bx lr
return_inf:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    add sp, sp, #12
    bx lr
a_nan:
    tst r0, #0x80000000
    beq signal_invalid
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq return_nan
    cmp ip, #0xff
    blt return_nan
    cmp r1, #0
    beq return_nan
b_nan:
    tst r1, #0x80000000
    beq signal_invalid
    mvn r0, #0x80000000
    add sp, sp, #12
    bx lr
signal_invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
return_nan:
    mvn r0, #0x80000000
    add sp, sp, #12
    bx lr
overflow_trap_after_round:
    add sp, sp, #12
    sub r0, r0, #0x60000000
    stmfd sp!, {lr}
    mov r1, #0x10000001
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
overflow_trap:
    add sp, sp, #12
    orr r0, r2, r0, lsr #8
    add r0, r0, #0x1f000000
    stmfd sp!, {lr}
    mov r1, #0x10000001
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
invalid_trap:
    ldmfd sp!, {r0, r1, r2}
    stmfd sp!, {lr}
    mov ip, #0x4000000
    orr r2, r2, ip
    bl __fp_trap_ff
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    add sp, sp, #12
    stmfd sp!, {lr}
    mov r1, #0x40000001
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
}
#else
#include "sdk/soft_float.h"

float _fadd(float a, float b) {
  u32 x = FpBits(a);
  u32 y = FpBits(b);

  if ((x ^ y) & FP_SIGN) {
    return __fsub_same_sign(x, y ^ FP_SIGN, a, b, FP_OP_ADD);
  }
  return __fadd_same_sign(x, y, a, b, FP_OP_ADD);
}

float __fadd_same_sign(u32 a, u32 b, float trapA, float trapB, u32 trapOp) {
  u32 sign;
  u32 expA;
  u32 expB;
  u32 manA;
  u32 manB;
  u32 shift;
  u32 sum;
  u32 bits;

  if (a < b) {
    u32 larger = b;
    b = a;
    a = larger;
  }
  /* From here |a| >= |b|, so b is no NaN unless a is one. */
  sign = a & FP_SIGN;
  expA = (a >> 23) & 0xff;
  expB = (b >> 23) & 0xff;
  if (expA == 0xff) {
    if (a << 9) {
      return FpNanResult(a, b, trapA, trapB, trapOp);
    }
    return FpFloat(sign | FP_INFINITY);
  }
  if (expA == 0) {
    /* Both zero or subnormal: the sum is exact and carries into the
       exponent field by itself. */
    return FpFloat(a + (b & ~FP_SIGN));
  }
  if ((b << 1) == 0) {
    return FpFloat(a);
  }

  /* Mantissas with the leading bit at bit 31; a subnormal b has exponent 1
     and no leading bit. */
  manA = (a << 8) | FP_SIGN;
  if (expB == 0) {
    manB = b << 8;
    expB = 1;
  } else {
    manB = (b << 8) | FP_SIGN;
  }
  shift = expA - expB;
  if (shift != 0) {
    u32 sticky;
    if (shift >= 28) {
      shift = 31;
    }
    sticky = (manB << (32 - shift)) != 0;
    manB = (manB >> shift) | sticky;
  }
  sum = manA + manB;
  if (sum < manA) {
    /* Carried out of bit 31: shift right, keeping the lost bit sticky. */
    sum = FP_SIGN | (sum >> 1) | (sum & 1);
    expA++;
    if (expA == 0xff) {
      return FpOverflow(
          sign, (sign | sum >> 8) + ((expA - 1 - FP_TRAP_EXP_ADJUST) << 23),
          FP_OP_ADD);
    }
  }
  bits = sign | ((sum << 1) >> 9) | (expA << 23);
  if ((sum & 0xff) == 0) {
    return FpFloat(bits);
  }
  return FpRoundInexact(bits, FpCompareHalf(sum & 0xff, 0x80), FP_INEXACT,
                        FP_OP_ADD);
}
#endif
