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
 * __fp_status returns the floating-point status word: bit 0 invalid,
 * bit 2 overflow, bit 4 inexact; bits 8, 10 and 12 enable the trap for each;
 * bits 22-23 the rounding mode (0 nearest, 1 toward +inf, 2 toward -inf,
 * 3 toward zero). A trap goes to __fp_trap_d or __fp_trap_dd with an
 * exception code.
 */

u32 *__fp_status(void);
double __fp_trap_d(double a, u32 op);
double __fp_trap_dd(double a, double b, u32 op);
#ifdef __MWERKS__
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
    bl __fp_status
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
    ldmfd sp!, {r4, lr}
    bx lr
overflow:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
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
    bl __fp_trap_d
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
    bl __fp_trap_d
    ldmfd sp!, {r4, lr}
    bx lr
trap_invalid:
    ldmfd sp!, {r0, r1, r2, r3}
    ldr ip, =0x04000091
    stmfd sp!, {ip}
    bl __fp_trap_dd
    add sp, sp, #4
    ldmfd sp!, {r4, lr}
    bx lr
trap_inexact:
    add sp, sp, #16
    ldr r2, =0x40000091
    bl __fp_trap_d
    ldmfd sp!, {r4, lr}
    bx lr
}
#else
#include <string.h>

/* The same addition on the bit patterns of the operands. The two halves
   take and return bits, so no NaN passes through a host double register;
   caller_b is the b that _dadd or _dsub received, which the invalid trap
   gets. Mantissas sit at the top of a u64 with the implicit 1 in bit 63,
   as in the assembly. */

#define FP_STICKY_INVALID 0x1
#define FP_STICKY_OVERFLOW 0x4
#define FP_STICKY_INEXACT 0x10
#define FP_TRAP_INVALID 0x100
#define FP_TRAP_OVERFLOW 0x400
#define FP_TRAP_INEXACT 0x1000
#define FP_ROUNDING(status) (((status) >> 22) & 3)
#define FP_ROUND_UP 1
#define FP_ROUND_DOWN 2
#define FP_ROUND_TO_ZERO 3
/* Operation words for the trap handlers: the exception in bits 26-30, 0x91
   for addition. */
#define FP_OP_INVALID_ADD 0x04000091
#define FP_OP_OVERFLOW_ADD 0x10000091
#define FP_OP_INEXACT_ADD 0x40000091

#define SIGN_BIT 0x8000000000000000ull
#define IMPLICIT_BIT 0x8000000000000000ull
#define FRACTION_MASK 0x000fffffffffffffull
#define QUIET_BIT 0x0008000000000000ull
#define INFINITY_BITS 0x7ff0000000000000ull
#define LARGEST_FINITE 0x7fefffffffffffffull
#define DEFAULT_NAN 0x7fffffffffffffffull
/* The trap handler gets an overflowed result with its exponent lowered by
   this much. */
#define OVERFLOW_EXPONENT_BIAS 0x600

/* The C reads a double's bits by copying it to a u64, so double must be the
   64-bit IEEE format in the same byte order as u64. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];

u64 __dadd_same_sign(u64 a, u64 b, u64 caller_b);
u64 __dsub_same_sign(u64 a, u64 b, u64 caller_b);

static u64 DoubleBits(double x) {
  u64 bits;

  memcpy(&bits, &x, sizeof(bits));
  return bits;
}

static double BitsDouble(u64 bits) {
  double x;

  memcpy(&x, &bits, sizeof(x));
  return x;
}

static int Exponent(u64 bits) { return (int)(bits >> 52) & 0x7ff; }

static BOOL IsNan(u64 bits) {
  return Exponent(bits) == 0x7ff && (bits & FRACTION_MASK) != 0;
}

static BOOL IsSignaling(u64 bits) {
  return IsNan(bits) && (bits & QUIET_BIT) == 0;
}

/* Shifts right by 1 to 63; any bit shifted out sets bit 0. */
static u64 ShiftRightSticky(u64 mantissa, int shift) {
  return (mantissa >> shift) | ((mantissa << (64 - shift)) != 0);
}

static u64 Invalid(u64 a, u64 caller_b) {
  u32* status = __fp_status();

  if (*status & FP_TRAP_INVALID) {
    return DoubleBits(
        __fp_trap_dd(BitsDouble(a), BitsDouble(caller_b), FP_OP_INVALID_ADD));
  }
  *status |= FP_STICKY_INVALID;
  return DEFAULT_NAN;
}

/* Whether rounding moves the truncated result away from zero. lost holds
   the bits cut off, from bit 31 down. */
static BOOL RoundsAway(u64 result, u32 lost, u32 status) {
  switch (FP_ROUNDING(status)) {
    case FP_ROUND_UP:
      return (result & SIGN_BIT) == 0;
    case FP_ROUND_DOWN:
      return (result & SIGN_BIT) != 0;
    case FP_ROUND_TO_ZERO:
      return FALSE;
    default:
      /* To nearest, ties to even. */
      return (lost & 0x80000000) != 0 &&
             ((u32)(lost << 1) != 0 || (result & 1) != 0);
  }
}

/* Rounds a sum whose cut-off bits are in lost. A sum that rounds up to
   infinity overflows. */
static u64 RoundSum(u64 result, u32 lost) {
  u32* status;

  if (lost == 0) {
    return result;
  }
  status = __fp_status();
  if (*status & FP_TRAP_INEXACT) {
    return DoubleBits(__fp_trap_d(BitsDouble(result), FP_OP_INEXACT_ADD));
  }
  *status |= FP_STICKY_INEXACT;
  if (!RoundsAway(result, lost, *status)) {
    return result;
  }
  result++;
  if (Exponent(result) != 0x7ff) {
    return result;
  }
  if (*status & FP_TRAP_OVERFLOW) {
    result -= (u64)OVERFLOW_EXPONENT_BIAS << 52;
    return DoubleBits(__fp_trap_d(BitsDouble(result), FP_OP_OVERFLOW_ADD));
  }
  *status |= FP_STICKY_OVERFLOW | FP_STICKY_INEXACT;
  return result;
}

/* The carry out of the mantissa took the exponent to 0x7ff. */
static u64 Overflow(u64 sign, u64 mantissa) {
  u32* status = __fp_status();

  if (*status & FP_TRAP_OVERFLOW) {
    u64 lowered = sign | (u64)(0x7ff - OVERFLOW_EXPONENT_BIAS) << 52 |
                  ((mantissa >> 11) & FRACTION_MASK);

    return DoubleBits(__fp_trap_d(BitsDouble(lowered), FP_OP_OVERFLOW_ADD));
  }
  *status |= FP_STICKY_OVERFLOW | FP_STICKY_INEXACT;
  switch (FP_ROUNDING(*status)) {
    case FP_ROUND_UP:
      return sign ? sign | LARGEST_FINITE : INFINITY_BITS;
    case FP_ROUND_DOWN:
      return sign ? sign | INFINITY_BITS : LARGEST_FINITE;
    case FP_ROUND_TO_ZERO:
      return sign | LARGEST_FINITE;
    default:
      return sign | INFINITY_BITS;
  }
}

double _dadd(double a, double b) {
  u64 a_bits = DoubleBits(a);
  u64 b_bits = DoubleBits(b);

  if ((a_bits ^ b_bits) & SIGN_BIT) {
    return BitsDouble(__dsub_same_sign(a_bits, b_bits ^ SIGN_BIT, b_bits));
  }
  return BitsDouble(__dadd_same_sign(a_bits, b_bits, b_bits));
}

/* a + b for a and b of the same sign. */
u64 __dadd_same_sign(u64 a, u64 b, u64 caller_b) {
  u64 big = a;
  u64 small = b;
  u64 sign = a & SIGN_BIT;
  u64 big_mantissa;
  u64 small_mantissa;
  u64 sum;
  int exponent;
  int small_exponent;
  int shift;

  /* The same sign bit, so this compares the magnitudes. */
  if (a < b) {
    big = b;
    small = a;
  }
  exponent = Exponent(big);
  small_exponent = Exponent(small);
  if (exponent == 0x7ff) {
    if (big & FRACTION_MASK) {
      /* Any NaN gives the default NaN; a signaling one is also invalid. */
      if (IsSignaling(big) || IsSignaling(small)) {
        return Invalid(a, caller_b);
      }
      return DEFAULT_NAN;
    }
    return big;
  }
  if ((small & ~SIGN_BIT) == 0) {
    return big;
  }
  if (exponent == 0) {
    /* Two subnormals: the fractions add exactly, and a carry into bit 52
       makes the exponent 1. */
    return sign | ((big & FRACTION_MASK) + (small & FRACTION_MASK));
  }
  big_mantissa = (big << 11) | IMPLICIT_BIT;
  if (small_exponent == 0) {
    small_mantissa = small << 11;
    small_exponent = 1;
  } else {
    small_mantissa = (small << 11) | IMPLICIT_BIT;
  }
  shift = exponent - small_exponent;
  if (shift != 0) {
    /* The assembly shifts by 63 from 56 on; the sticky bit makes that the
       same as the full shift. */
    small_mantissa = ShiftRightSticky(small_mantissa, shift >= 56 ? 63 : shift);
  }
  sum = big_mantissa + small_mantissa;
  if (sum < big_mantissa) {
    /* Carry out of bit 63: shift it back in, keeping the lost bit sticky. */
    exponent++;
    sum = IMPLICIT_BIT | (sum >> 1) | (sum & 1);
    if (exponent == 0x7ff) {
      return Overflow(sign, sum);
    }
  }
  return RoundSum(sign | (u64)exponent << 52 | ((sum >> 11) & FRACTION_MASK),
                  (u32)sum << 21);
}
#endif
