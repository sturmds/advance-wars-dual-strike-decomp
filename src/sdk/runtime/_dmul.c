#include "types.h"
#include "sdk/fp_status.h"

/*
 * Metrowerks runtime, hand-written ARM assembly: IEEE double multiplication
 * for the soft-float ABI (a in r0:r1, b in r2:r3, product in r0:r1).
 * Status word as in _dadd.c, plus bit 3 underflow and bit 11 its trap
 * enable.
 *
 * _drsb (b - a) shares this file because it sits right after _dmul's
 * literal pool and falls through into _dsub, the next object. dsd cannot
 * give it a function symbol of its own (it refuses a function that starts
 * with an eor), so symbols.txt lists it as a label.
 */

double __fp_trap_d(double a, u32 op);
double __fp_trap_dd(double a, double b, u32 op);

#ifdef __MWERKS__
asm double _dmul(double a, double b)
{
    /* lr keeps the sign of the product; ip and r4 the exponents. */
    stmfd sp!, {r0, r1, r2, r3, r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov ip, r1, lsr #20
    mov r1, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r6, ip, lsl #21
    cmnne r6, #0x200000
    beq a_not_normal
    orr r1, r1, #0x80000000
    bic ip, ip, #0x800
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq b_not_normal
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
multiply:
    /* 64x64-bit product of the mantissas; the low word folds into a sticky bit. */
    add ip, r4, ip
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt normalized
    sub ip, ip, #1
    adds r0, r0, r0
    adc r1, r1, r1
normalized:
    add ip, ip, #2
    subs ip, ip, #0x400
    bmi underflow
    beq underflow
    mov r6, ip, lsl #20
    cmn r6, #0x100000
    bmi overflow
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
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
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
round_up:
    adds r0, r0, #1
    adc r1, r1, #0
    mov r2, r1, lsl #1
    cmn r2, #0x200000
    addcc sp, sp, #16
    ldmccfd sp!, {r4, r5, r6, r7, lr}
    bxcc lr
    tst r3, #0x400
    bne trap_round_overflow
    orr r3, r3, #20
    str r3, [ip]
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
toward_plus:
    cmp lr, #0
    beq round_up
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    cmp lr, #0
    bne round_up
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
a_not_normal:
    bics ip, ip, #0x800
    beq a_subnormal
    orrs r6, r0, r1, lsl #1
    bne a_is_nan
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq a_inf_b_small
    cmn r5, #0x200000
    bne return_inf
    orrs r5, r2, r3, lsl #1
    beq return_inf
    b b_is_nan
a_inf_b_small:
    orrs r5, r3, r2
    beq signal_invalid
    b return_inf
b_not_normal:
    bics r4, r4, #0x800
    beq b_subnormal
    orrs r6, r2, r3, lsl #1
    bne b_is_nan
    b return_inf
a_subnormal:
    orrs r6, r0, r1, lsl #1
    beq a_zero
    mov ip, #1
    cmp r1, #0
    bne a_normalize_clz
    sub ip, ip, #32
    movs r1, r0
    mov r0, #0
    bmi a_normalized
a_normalize_clz:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #32
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #32
    mov r0, r0, lsl r6
    sub ip, ip, r6
a_normalized:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    cmnne r5, #0x200000
    beq b_not_normal
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b multiply
a_zero:
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq return_zero
    cmn r5, #0x200000
    bne return_zero
    orrs r6, r2, r3, lsl #1
    beq signal_invalid
    b b_is_nan
b_subnormal:
    orrs r5, r2, r3, lsl #1
    beq return_zero
    mov r4, #1
    cmp r3, #0
    bne b_normalize_clz
    sub r4, r4, #32
    movs r3, r2
    mov r2, #0
    bmi multiply
b_normalize_clz:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #32
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #32
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b multiply
underflow:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r2, [r2]
    tst r2, #0x800
    bne trap_underflow
    cmn ip, #52
    beq underflow_half
    bmi underflow_to_zero
    mov r2, r1
    mov r3, r0
    add r4, ip, #52
    cmp r4, #32
    movge r2, r3
    movge r3, #0
    subge r4, r4, #32
    rsb r5, r4, #32
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb ip, ip, #12
    cmp ip, #32
    movge r0, r1
    movge r1, #0
    subge ip, ip, #32
    rsb r4, ip, #32
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    cmp r2, #0
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    bne tiny_toward_plus
    tst r2, #0x80000000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
tiny_toward_plus:
    cmp lr, #0
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, #1
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
tiny_toward_minus_or_zero:
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, r1, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
underflow_half:
    orr r0, r0, r1, lsl #1
    b round_tiny
return_inf:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
a_is_nan:
    tst r1, #0x40000000
    beq signal_invalid
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r5, r4, lsl #21
    beq return_default_nan
    cmn r5, #0x200000
    bne return_default_nan
    orrs r5, r2, r3, lsl #1
    beq return_default_nan
b_is_nan:
    mov r1, r3
    mov r3, r1
    mvn r0, #0
    bic r1, r0, #0x80000000
    tst r3, #0x40000000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
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
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
signal_invalid:
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
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
round_tiny:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    bne tiny_toward_plus
    cmp r2, #0
    addne r0, r0, #1
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
underflow_to_zero:
    mov r1, lr
    mov r0, #0
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne trap_inexact
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_or_zero
    tst r3, #0x400000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    cmp r1, #0
    addeq r0, r0, #1
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
overflow:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x400
    bne trap_overflow
    orr r3, r3, #20
    str r3, [r2]
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    tst r3, #0x800000
    bne overflow_toward_minus
    tst r3, #0x400000
    addeq sp, sp, #16
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    subs r0, r0, lr, lsr #31
    sbc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
overflow_toward_minus:
    subs r0, r0, #1
    sbc r1, r1, #0
    tst r3, #0x400000
    addne sp, sp, #16
    ldmnefd sp!, {r4, r5, r6, r7, lr}
    bxne lr
    adds r0, r0, lr, lsr #31
    adc r1, r1, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
return_zero:
    mov r1, lr
    mov r0, #0
    add sp, sp, #16
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_round_overflow:
    add sp, sp, #16
    sub r1, r1, #0x60000000
    ldr r2, =0x10000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_overflow:
    add sp, sp, #16
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    sub ip, ip, #0x600
    orr r1, r1, ip, lsl #20
    ldr r2, =0x10000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_underflow:
    add sp, sp, #16
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    add ip, ip, #0x600
    orr r1, r1, ip, lsl #20
    ldr r2, =0x20000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_invalid:
    ldmfd sp!, {r0, r1, r2, r3}
    ldr ip, =0x04000093
    stmfd sp!, {ip}
    bl __fp_trap_dd
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
trap_inexact:
    add sp, sp, #16
    ldr r2, =0x40000093
    bl __fp_trap_d
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}

/* Swaps a and b with three eors each, then runs on into _dsub. */
asm double _drsb(double a, double b)
{
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}
#else
#include <string.h>

/* The same multiplication on the bit patterns of the operands. Mantissas
   sit at the top of a u64 with the implicit 1 in bit 63, as in the
   assembly. */

#define FP_STICKY_INVALID 0x1
#define FP_STICKY_OVERFLOW 0x4
#define FP_STICKY_UNDERFLOW 0x8
#define FP_STICKY_INEXACT 0x10
#define FP_TRAP_INVALID 0x100
#define FP_TRAP_OVERFLOW 0x400
#define FP_TRAP_UNDERFLOW 0x800
#define FP_TRAP_INEXACT 0x1000
#define FP_ROUNDING(status) (((status) >> 22) & 3)
#define FP_ROUND_UP 1
#define FP_ROUND_DOWN 2
#define FP_ROUND_TO_ZERO 3
/* Operation words for the trap handlers: the exception in bits 26-30, 0x93
   for multiplication. */
#define FP_OP_INVALID_MUL 0x04000093
#define FP_OP_OVERFLOW_MUL 0x10000093
#define FP_OP_UNDERFLOW_MUL 0x20000093
#define FP_OP_INEXACT_MUL 0x40000093

#define SIGN_BIT 0x8000000000000000ull
#define IMPLICIT_BIT 0x8000000000000000ull
#define FRACTION_MASK 0x000fffffffffffffull
#define QUIET_BIT 0x0008000000000000ull
#define INFINITY_BITS 0x7ff0000000000000ull
#define LARGEST_FINITE 0x7fefffffffffffffull
#define DEFAULT_NAN 0x7fffffffffffffffull
/* The trap handler gets an overflowed or underflowed result with its
   exponent moved this much back into range. */
#define TRAP_EXPONENT_BIAS 0x600

/* The C reads a double's bits by copying it to a u64, so double must be the
   64-bit IEEE format in the same byte order as u64. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];

double _dsub(double a, double b);

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

static BOOL IsZero(u64 bits) { return (bits & ~SIGN_BIT) == 0; }

/* The top-aligned mantissa of a finite nonzero double and its exponent. A
   subnormal is shifted up until its top bit reaches bit 63, and its
   exponent goes below 1 by the same amount. */
static u64 Mantissa(u64 bits, int* exponent) {
  u64 mantissa = bits << 11;

  *exponent = Exponent(bits);
  if (*exponent != 0) {
    return mantissa | IMPLICIT_BIT;
  }
  *exponent = 1;
  while ((mantissa & IMPLICIT_BIT) == 0) {
    mantissa <<= 1;
    (*exponent)--;
  }
  return mantissa;
}

/* The top 64 bits of the 128-bit product, with bit 0 set when any of the
   low 64 bits is. */
static u64 MultiplySticky(u64 a, u64 b) {
  u64 a_low = (u32)a;
  u64 a_high = a >> 32;
  u64 b_low = (u32)b;
  u64 b_high = b >> 32;
  u64 low = a_low * b_low;
  u64 middle_a = a_high * b_low;
  u64 middle_b = a_low * b_high;
  u64 middle = (low >> 32) + (u32)middle_a + (u32)middle_b;
  u64 high =
      a_high * b_high + (middle_a >> 32) + (middle_b >> 32) + (middle >> 32);

  return high | (((u32)middle | (u32)low) != 0);
}

static u64 Invalid(double a, double b) {
  u32* status = __fp_status();

  if (*status & FP_TRAP_INVALID) {
    return DoubleBits(__fp_trap_dd(a, b, FP_OP_INVALID_MUL));
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

/* Rounds a normal product whose cut-off bits are in lost. A product that
   rounds up to infinity overflows. */
static u64 RoundProduct(u64 result, u32 lost) {
  u32* status;

  if (lost == 0) {
    return result;
  }
  status = __fp_status();
  if (*status & FP_TRAP_INEXACT) {
    return DoubleBits(__fp_trap_d(BitsDouble(result), FP_OP_INEXACT_MUL));
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
    result -= (u64)TRAP_EXPONENT_BIAS << 52;
    return DoubleBits(__fp_trap_d(BitsDouble(result), FP_OP_OVERFLOW_MUL));
  }
  *status |= FP_STICKY_OVERFLOW | FP_STICKY_INEXACT;
  return result;
}

/* The exponent is 0x7ff or more. */
static u64 Overflow(u64 sign, int exponent, u64 mantissa) {
  u32* status = __fp_status();

  if (*status & FP_TRAP_OVERFLOW) {
    u64 lowered = sign | (u64)(exponent - TRAP_EXPONENT_BIAS) << 52 |
                  ((mantissa >> 11) & FRACTION_MASK);

    return DoubleBits(__fp_trap_d(BitsDouble(lowered), FP_OP_OVERFLOW_MUL));
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

/* The exponent is 0 or less: the product becomes subnormal or zero. Only a
   result that loses bits sets the underflow flag. */
static u64 Underflow(u64 sign, int exponent, u64 mantissa) {
  u32* status = __fp_status();
  u64 result;
  u64 low_bits;
  u32 lost;

  if (*status & FP_TRAP_UNDERFLOW) {
    u64 raised = sign | (u64)(exponent + TRAP_EXPONENT_BIAS) << 52 |
                 ((mantissa >> 11) & FRACTION_MASK);

    return DoubleBits(__fp_trap_d(BitsDouble(raised), FP_OP_UNDERFLOW_MUL));
  }
  if (exponent < -52) {
    /* Below half the smallest subnormal: rounds to nearest as 0. */
    result = sign;
    lost = 1;
  } else if (exponent == -52) {
    /* From half the smallest subnormal up to it. */
    result = sign;
    lost = 0x80000000 | ((mantissa << 1) != 0);
  } else {
    result = sign | mantissa >> (12 - exponent);
    low_bits = mantissa << (52 + exponent);
    lost = (u32)(low_bits >> 32) | ((u32)low_bits != 0);
    if (lost == 0) {
      return result;
    }
  }
  if (*status & FP_TRAP_INEXACT) {
    return DoubleBits(__fp_trap_d(BitsDouble(result), FP_OP_INEXACT_MUL));
  }
  *status |= FP_STICKY_UNDERFLOW | FP_STICKY_INEXACT;
  if (RoundsAway(result, lost, *status)) {
    result++;
  }
  return result;
}

double _dmul(double a, double b) {
  u64 a_bits = DoubleBits(a);
  u64 b_bits = DoubleBits(b);
  u64 sign = (a_bits ^ b_bits) & SIGN_BIT;
  u64 mantissa;
  int exponent;
  int b_exponent;

  if (IsNan(a_bits) || IsNan(b_bits)) {
    /* Any NaN gives the default NaN; a signaling one is also invalid. */
    if (IsSignaling(a_bits) || IsSignaling(b_bits)) {
      return BitsDouble(Invalid(a, b));
    }
    return BitsDouble(DEFAULT_NAN);
  }
  if (Exponent(a_bits) == 0x7ff || Exponent(b_bits) == 0x7ff) {
    if (IsZero(a_bits) || IsZero(b_bits)) {
      /* Infinity times zero. */
      return BitsDouble(Invalid(a, b));
    }
    return BitsDouble(sign | INFINITY_BITS);
  }
  if (IsZero(a_bits) || IsZero(b_bits)) {
    return BitsDouble(sign);
  }
  mantissa = MultiplySticky(Mantissa(a_bits, &exponent),
                            Mantissa(b_bits, &b_exponent));
  /* Both mantissas are in [1, 2), so the product is in [1, 4). */
  exponent += b_exponent - 1022;
  if ((mantissa & IMPLICIT_BIT) == 0) {
    mantissa <<= 1;
    exponent--;
  }
  if (exponent <= 0) {
    return BitsDouble(Underflow(sign, exponent, mantissa));
  }
  if (exponent >= 0x7ff) {
    return BitsDouble(Overflow(sign, exponent, mantissa));
  }
  return BitsDouble(RoundProduct(
      sign | (u64)exponent << 52 | ((mantissa >> 11) & FRACTION_MASK),
      (u32)mantissa << 21));
}

/* b - a. */
double _drsb(double a, double b) { return _dsub(b, a); }
#endif
