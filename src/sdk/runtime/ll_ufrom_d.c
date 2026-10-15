#include "types.h"
#include "sdk/fp_status.h"

extern double __fp_trap_d(double a, u32 op);

#ifdef __MWERKS__
/*
 * Converts a double (r0 low word, r1 high word) to an unsigned 64-bit
 * integer (r0 low, r1 high), rounding toward zero. Negative inputs give 0.
 * Values of 2^64 and above, and NaN, set the invalid flag (status bit 0) or
 * call the trap handler when bit 8 is set, and return all ones. An inexact
 * result sets bit 4 or calls the trap handler when bit 12 is set.
 */
asm u64 _ll_ufrom_d(double x)
{
    tst r1, #0x80000000
    bne negative
    ldr r2, =0x43e
    subs r2, r2, r1, lsr #20
    blt invalid
    cmp r2, #64
    bge below_one
    mov ip, r1, lsl #11
    orr ip, ip, #0x80000000
    orr ip, ip, r0, lsr #21
    cmp r2, #32
    ble small_shift
    sub r2, r2, #32
    mov r3, r0, lsl #11
    mov r1, #0
    mov r0, ip, lsr r2
    rsb r2, r2, #32
    orrs ip, r3, ip, lsl r2
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
small_shift:
    mov r3, r0, lsl #11
    mov r1, ip, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #32
    orr r0, r0, ip, lsl r2
    movs ip, r3, lsl r2
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
below_one:
    orrs r1, r1, r0
    bxeq lr
    mov r1, #0
    mov r0, #0
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
negative:
    cmn r1, #0x100000
    cmpeq r0, #0
    bhi invalid
    mov r1, #0
    mov r0, #0
    bx lr
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
    mvn r1, #0
    mvn r0, #0
    bx lr
invalid_trap:
    ldr r2, =0x04c000f8
    stmfd sp!, {lr}
    bl __fp_trap_d
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    ldr r2, =0x40c000f8
    stmfd sp!, {lr}
    bl __fp_trap_d
    ldmfd sp!, {lr}
    bx lr
}
#else
#include <string.h>

#define FP_STICKY_INVALID 0x1
#define FP_STICKY_INEXACT 0x10
#define FP_TRAP_INVALID 0x100
#define FP_TRAP_INEXACT 0x1000
/* Operation words for the trap handlers: the exception in bits 26-30,
   rounding toward zero in bits 22-23, 0xf8 for this conversion. */
#define FP_OP_INVALID_LL_UFROM_D 0x04c000f8
#define FP_OP_INEXACT_LL_UFROM_D 0x40c000f8

/* The C reads a double's bits by copying it to a u64, so double must be the
   64-bit IEEE format in the same byte order as u64. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];

static u64 DoubleBits(double x) {
  u64 bits;

  memcpy(&bits, &x, sizeof(bits));
  return bits;
}

/* The assembly passes the result to __fp_trap_d where a double goes, and
   returns the two words it gets back. */
static u64 Inexact(u64 result) {
  u32* status = __fp_status();
  double bits;

  if (*status & FP_TRAP_INEXACT) {
    memcpy(&bits, &result, sizeof(bits));
    return DoubleBits(__fp_trap_d(bits, FP_OP_INEXACT_LL_UFROM_D));
  }
  *status |= FP_STICKY_INEXACT;
  return result;
}

static u64 Invalid(double x) {
  u32* status = __fp_status();

  if (*status & FP_TRAP_INVALID) {
    return DoubleBits(__fp_trap_d(x, FP_OP_INVALID_LL_UFROM_D));
  }
  *status |= FP_STICKY_INVALID;
  return 0xffffffffffffffffull;
}

u64 _ll_ufrom_d(double x) {
  u64 bits = DoubleBits(x);
  u64 mantissa;
  int shift;

  if (bits >> 63) {
    /* A negative NaN is invalid; anything else negative, -infinity too,
       gives 0 without a flag. */
    return bits > 0xfff0000000000000ull ? Invalid(x) : 0;
  }
  /* x >> shift is the integer part; 1 <= x < 2^64 gives 0 to 63. */
  shift = 0x43e - (int)(bits >> 52);
  if (shift < 0) {
    /* 2^64 or more, infinity or NaN. */
    return Invalid(x);
  }
  if (shift >= 64) {
    /* x < 1. */
    return bits == 0 ? 0 : Inexact(0);
  }
  mantissa = (bits << 11) | 0x8000000000000000ull;
  if (shift == 0 || (mantissa << (64 - shift)) == 0) {
    return mantissa >> shift;
  }
  return Inexact(mantissa >> shift);
}
#endif
