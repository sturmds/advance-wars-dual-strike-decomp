#include "types.h"

#include "sdk/fp_status.h"

/*
 * Converts a float to a signed 32-bit integer, rounding toward zero.
 * __fp_status returns the soft-float status word: flags in bits 0-4
 * (bit 4 inexact), trap enables in bits 8-12 (bit 12 inexact). An inexact
 * result sets the flag, or calls the trap handler when its enable is set.
 * Values of 2^31 and above, and negative 2^31 exactly, continue in
 * func_02128438 (the next function in the ROM).
 * The branches into func_02128438 are encoded with dcd because the
 * assembler cannot branch into another function.
 */
#ifdef __MWERKS__
asm int _ffix(float x)
{
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #23
    ble out_of_range
    cmp r2, #32
    bge below_one
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    rsb r2, r2, #32
    movs r1, r1, lsl r2
    bxeq lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x1000
    dcd 0x1a000022 // bne func_02128438 + 0x44 (inexact trap)
    orr r3, r3, #0x10
    str r3, [r2]
    bx lr
below_one:
    movs r0, r1
    bxeq lr
    mov r0, #0
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl __fp_status
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x1000
    dcd 0x1a000015 // bne func_02128438 + 0x44 (inexact trap)
    orr r3, r3, #0x10
    str r3, [r1]
    bx lr
out_of_range:
    cmp r0, #0xcf000000
    dcd 0x0a000008 // beq func_02128438 + 0x24 (exactly -2^31)
}
#else
#include "sdk/soft_float.h"

int func_02128438(float x);

int _ffix(float x) {
  u32 bits = FpBits(x);
  u32 abs = bits & ~FP_SIGN;
  int shift = FP_BIAS + 31 - (int)(abs >> 23);
  u32 man;
  u32 result;
  u32* status;

  if (shift <= 0) {
    /* 2^31 or more, infinity or NaN; -2^31 itself converts exactly. */
    if (bits == 0xcf000000) {
      return (int)0x80000000;
    }
    return func_02128438(x);
  }
  if (shift >= 32) {
    if (abs == 0) {
      return 0;
    }
    result = 0;
  } else {
    man = (abs << 8) | FP_SIGN;
    result = man >> shift;
    if (bits & FP_SIGN) {
      result = 0u - result;
    }
    if ((man << (32 - shift)) == 0) {
      return (int)result;
    }
  }
  status = __fp_status();
  if (*status & FP_TRAP_ENABLE(FP_INEXACT)) {
    /* The trap gets the integer's bits as a float, and its result is
       returned the same way. */
    return (int)FpBits(__fp_trap_f(
        FpFloat(result), FP_TRAP_WORD(FP_INEXACT, FP_OP_FLOAT_TO_INT)));
  }
  *status |= FP_INEXACT;
  return (int)result;
}
#endif
