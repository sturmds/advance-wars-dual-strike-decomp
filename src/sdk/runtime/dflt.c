#include "types.h"

/*
 * Converts a signed 32-bit integer to a double (result in r0 low, r1 high).
 * Always exact. The sign is kept in r2, the magnitude is shifted up with clz
 * and the exponent is 0x41e minus the shift.
 */
#ifdef __MWERKS__
asm double _dflt(int x)
{
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #30
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    movs r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
#else
#include <string.h>

/* The C writes a double's bits by copying a u64 to it, so double must be
   the 64-bit IEEE format in the same byte order as u64. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];

double _dflt(int x) {
  u64 sign = x < 0 ? 0x8000000000000000ull : 0;
  /* The magnitude as unsigned, so INT_MIN gives 0x80000000. */
  u32 magnitude = x < 0 ? 0u - (u32)x : (u32)x;
  int exponent = 0x41e;
  u64 bits = 0;
  double result;

  if (magnitude != 0) {
    while ((magnitude & 0x80000000) == 0) {
      magnitude <<= 1;
      exponent--;
    }
    bits = sign | (u64)exponent << 52 | (u64)(magnitude & 0x7fffffff) << 21;
  }
  memcpy(&result, &bits, sizeof(result));
  return result;
}
#endif
