#include "types.h"

/*
 * Converts an unsigned 32-bit integer to a double (result in r0 low, r1
 * high). Always exact, so it sets no status flags. The value is shifted
 * up with clz until its top bit is set; that bit becomes the hidden bit and
 * the exponent is 0x41e minus the shift.
 */
#ifdef __MWERKS__
asm double _dfltu(u32 x)
{
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #30
    bmi normalised
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
normalised:
    mov r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
#else
#include <string.h>

/* The C writes a double's bits by copying a u64 to it, so double must be
   the 64-bit IEEE format in the same byte order as u64. */
typedef char DoubleIsEightBytes[sizeof(double) == 8 ? 1 : -1];

double _dfltu(u32 x) {
  int exponent = 0x41e;
  u64 bits = 0;
  double result;

  if (x != 0) {
    while ((x & 0x80000000) == 0) {
      x <<= 1;
      exponent--;
    }
    bits = (u64)exponent << 52 | (u64)(x & 0x7fffffff) << 21;
  }
  memcpy(&result, &bits, sizeof(result));
  return result;
}
#endif
