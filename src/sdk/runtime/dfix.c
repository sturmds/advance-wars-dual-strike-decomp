#include "types.h"
#include "sdk/fp_status.h"

#ifdef __MWERKS__
/*
 * Converts a double (r0 low word, r1 high word) to a signed 32-bit integer,
 * rounding toward zero. Same status word and trap scheme as _ffix: an
 * inexact result sets bit 4 or calls the trap handler when bit 12 is set;
 * an out-of-range value returns 0x7fffffff or 0x80000000 and sets the
 * invalid flag (bit 0) or calls the trap handler when bit 8 is set.
 */
asm int _dfix(double x)
{
    bic r3, r1, #0x80000000
    ldr r2, =0x41e
    subs r2, r2, r3, lsr #20
    ble out_of_range
    cmp r2, #32
    bge below_one
    mov r3, r1, lsl #11
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    mov ip, r0, lsl #11
    cmp r1, #0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0
    rsb r2, r2, #32
    orrs ip, ip, r3, lsl r2
    bxeq lr
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl __fp_status
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
below_one:
    orrs r0, r0, r1, lsl #1
    bxeq lr
    mov r0, #0
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl __fp_status
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
out_of_range:
    ldr r2, =0xc1e00000
    cmp r1, r2
    beq maybe_min_int
invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
    mvn r0, r1, asr #31
    add r0, r0, #0x80000000
    bx lr
maybe_min_int:
    cmp r0, #0x200000
    bcs invalid
    cmp r0, #0
    mov r0, #0x80000000
    bxeq lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #0x10
    str r3, [r2]
    bx lr
invalid_trap:
    ldr r2, =0x04c000c8
    stmfd sp!, {lr}
    bl FpTrap_DoubleToInt
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    ldr r1, =0x40c000c8
    stmfd sp!, {lr}
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
}
#else
#include <limits.h>
#include <string.h>

#define FP_STICKY_INVALID 0x1
#define FP_STICKY_INEXACT 0x10
#define FP_TRAP_INVALID 0x100
#define FP_TRAP_INEXACT 0x1000
/* Operation words for the trap handlers: the exception in bits 26-30,
   rounding toward zero in bits 22-23, 0xc8 for this conversion. */
#define FP_OP_INVALID_DFIX 0x04c000c8
#define FP_OP_INEXACT_DFIX 0x40c000c8

/* The C reads a double's bits by copying it to a u64, so double must be the
   64-bit IEEE format in the same byte order as u64. The inexact trap gets
   the integer's bits where a float goes, so float must be 4 bytes. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];
typedef char FloatIsFourBytes[sizeof(float) == 4 ? 1 : -1];

/* The assembly passes the integer to __fp_trap_f in r0, where a float
   goes, and returns the r0 it gets back. */
static int Inexact(int result) {
  u32* status = __fp_status();
  float bits;

  if (*status & FP_TRAP_INEXACT) {
    memcpy(&bits, &result, sizeof(bits));
    bits = __fp_trap_f(bits, FP_OP_INEXACT_DFIX);
    memcpy(&result, &bits, sizeof(result));
    return result;
  }
  *status |= FP_STICKY_INEXACT;
  return result;
}

int _dfix(double x) {
  u64 bits;
  u64 mantissa;
  int shift;
  int result;

  memcpy(&bits, &x, sizeof(bits));
  /* x >> shift + 32 is the integer part; 1 <= |x| < 2^31 gives 1 to 31. */
  shift = 0x41e - (int)((bits >> 52) & 0x7ff);
  if (shift <= 0) {
    if ((bits >> 32) == 0xc1e00000 && (u32)bits < 0x200000) {
      /* -2^31 - f with 0 <= f < 1. */
      return (u32)bits == 0 ? INT_MIN : Inexact(INT_MIN);
    }
    /* Too large, infinity or NaN. */
    if (*__fp_status() & FP_TRAP_INVALID) {
      return FpTrap_DoubleToInt(x, FP_OP_INVALID_DFIX);
    }
    *__fp_status() |= FP_STICKY_INVALID;
    return (bits >> 63) ? INT_MIN : INT_MAX;
  }
  if (shift >= 32) {
    /* |x| < 1. */
    return (bits << 1) == 0 ? 0 : Inexact(0);
  }
  mantissa = (bits << 11) | 0x8000000000000000ull;
  result = (int)(mantissa >> (shift + 32));
  if (bits >> 63) {
    result = -result;
  }
  if ((mantissa << (32 - shift)) == 0) {
    return result;
  }
  return Inexact(result);
}
#endif
