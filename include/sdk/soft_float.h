#ifndef SOFT_FLOAT_H
#define SOFT_FLOAT_H

#include "sdk/fp_status.h"
#include "types.h"

/*
 * Shared by the plain C beside the single-precision soft-float helpers in
 * src/sdk/runtime/ (docs/workflow.md, "Portable C beside asm"). Only the
 * #else side of those files includes it; mwcc builds the assembly.
 *
 * The helpers work on IEEE single bit patterns. __fp_status returns the
 * status word: sticky exception flags in bits 0-4, the trap enable of each
 * flag 8 bits higher, and the rounding mode in bits 22-23.
 */

/* The C reads a float's bits through a u32. */
typedef char FpSizeCheck[sizeof(float) == sizeof(u32) ? 1 : -1];

#define FP_INVALID 0x01
#define FP_DIVIDE_BY_ZERO 0x02
#define FP_OVERFLOW 0x04
#define FP_UNDERFLOW 0x08
#define FP_INEXACT 0x10
#define FP_TRAP_ENABLE(flag) ((u32)(flag) << 8)
#define FP_ROUNDING(status) (((status) >> 22) & 3)

#define FP_ROUND_NEAREST 0 /* ties to even */
#define FP_ROUND_UPWARD 1
#define FP_ROUND_DOWNWARD 2
#define FP_ROUND_TOWARD_ZERO 3

/* The word passed to __fp_trap_f and __fp_trap_ff: the exception flag in
   bits 26-30, the operation code below it. The conversions use wider codes,
   copied from the assembly. */
#define FP_TRAP_WORD(flag, op) ((u32)(flag) << 26 | (op))
#define FP_OP_ADD 1
#define FP_OP_SUB 2
#define FP_OP_MUL 3
#define FP_OP_DIV 4
#define FP_OP_FLOAT_TO_INT 0xc00048
#define FP_OP_INT_TO_FLOAT 0x208
#define FP_OP_UINT_TO_FLOAT 0x308

#define FP_BIAS 0x7f
/* An overflow or underflow trap gets the result with its exponent moved by
   this much toward the middle of the range. */
#define FP_TRAP_EXP_ADJUST 0xc0

#define FP_SIGN 0x80000000u
#define FP_INFINITY 0x7f800000u
#define FP_QUIET 0x00400000u
/* The NaN every helper returns. */
#define FP_DEFAULT_NAN 0x7fffffffu

/* Defined in fadd.c and fsub.c: |a| + |b| and |a| - |b| for a and b of the
   same sign, which the result keeps (negated when |b| > |a|). An invalid
   operation traps with the caller's operands and operation number. */
float __fadd_same_sign(u32 a, u32 b, float trapA, float trapB, u32 trapOp);
float __fsub_same_sign(u32 a, u32 b, float trapA, float trapB, u32 trapOp);

static inline u32 FpBits(float f) {
  union {
    float f;
    u32 bits;
  } v;
  v.f = f;
  return v.bits;
}

static inline float FpFloat(u32 bits) {
  union {
    float f;
    u32 bits;
  } v;
  v.bits = bits;
  return v.f;
}

static inline int FpCountLeadingZeros(u32 x) {
  int n = 0;
  if (x == 0) {
    return 32;
  }
  while (!(x & 0x80000000u)) {
    x <<= 1;
    n++;
  }
  return n;
}

/* x >> n, 0 for n of 32 or more, as ARM's shift by register gives. */
static inline u32 FpShiftRight(u32 x, u32 n) { return n < 32 ? x >> n : 0; }

/* -1, 0 or 1 as the dropped bits rest are below, at or above half. */
static inline int FpCompareHalf(u32 rest, u32 half) {
  return rest < half ? -1 : rest > half;
}

static inline int FpIsSignalingNan(u32 x) {
  return (x & FP_INFINITY) == FP_INFINITY && (x << 9) != 0 && !(x & FP_QUIET);
}

/* Invalid operation: traps through __fp_trap_ff when enabled, otherwise sets
   the flag and returns the default NaN. */
static inline float FpInvalid(float trapA, float trapB, u32 op) {
  u32* status = __fp_status();
  if (*status & FP_TRAP_ENABLE(FP_INVALID)) {
    return __fp_trap_ff(trapA, trapB, FP_TRAP_WORD(FP_INVALID, op));
  }
  *status |= FP_INVALID;
  return FpFloat(FP_DEFAULT_NAN);
}

/* An operation with a NaN operand: invalid when either is a signaling NaN,
   otherwise the default NaN without a flag. */
static inline float FpNanResult(u32 a, u32 b, float trapA, float trapB,
                                u32 op) {
  if (FpIsSignalingNan(a) || FpIsSignalingNan(b)) {
    return FpInvalid(trapA, trapB, op);
  }
  return FpFloat(FP_DEFAULT_NAN);
}

/* A result too large for the format: traps with trapBits (the result with
   its exponent lowered by FP_TRAP_EXP_ADJUST) when enabled, otherwise sets overflow and
   inexact and returns infinity or the largest finite value of the sign, by
   the rounding mode. */
static inline float FpOverflow(u32 sign, u32 trapBits, u32 op) {
  u32* status = __fp_status();
  u32 bits = sign | FP_INFINITY;

  if (*status & FP_TRAP_ENABLE(FP_OVERFLOW)) {
    return __fp_trap_f(FpFloat(trapBits), FP_TRAP_WORD(FP_OVERFLOW, op));
  }
  *status |= FP_OVERFLOW | FP_INEXACT;
  switch (FP_ROUNDING(*status)) {
    case FP_ROUND_UPWARD:
      if (sign) {
        bits--;
      }
      break;
    case FP_ROUND_DOWNWARD:
      if (!sign) {
        bits--;
      }
      break;
    case FP_ROUND_TOWARD_ZERO:
      bits--;
      break;
  }
  return FpFloat(bits);
}

/*
 * Rounds bits, an inexact result truncated toward zero, by the rounding mode.
 * vsHalf compares the dropped part with half a unit in the last place
 * (FpCompareHalf). Sets flags (inexact, plus underflow for a tiny result)
 * unless the inexact trap is enabled, which gets the truncated result. A
 * result that rounds up to infinity overflows.
 */
static inline float FpRoundInexact(u32 bits, int vsHalf, u32 flags, u32 op) {
  u32* status = __fp_status();
  int up;

  if (*status & FP_TRAP_ENABLE(FP_INEXACT)) {
    return __fp_trap_f(FpFloat(bits), FP_TRAP_WORD(FP_INEXACT, op));
  }
  *status |= flags;
  switch (FP_ROUNDING(*status)) {
    case FP_ROUND_NEAREST:
      up = vsHalf > 0 || (vsHalf == 0 && (bits & 1));
      break;
    case FP_ROUND_UPWARD:
      up = !(bits & FP_SIGN);
      break;
    case FP_ROUND_DOWNWARD:
      up = (bits & FP_SIGN) != 0;
      break;
    default:
      up = 0;
      break;
  }
  if (up) {
    bits++;
    if ((bits << 1) >= (FP_INFINITY << 1)) {
      if (*status & FP_TRAP_ENABLE(FP_OVERFLOW)) {
        return __fp_trap_f(FpFloat(bits - (FP_TRAP_EXP_ADJUST << 23)),
                           FP_TRAP_WORD(FP_OVERFLOW, op));
      }
      *status |= FP_OVERFLOW | FP_INEXACT;
    }
  }
  return FpFloat(bits);
}

#endif
