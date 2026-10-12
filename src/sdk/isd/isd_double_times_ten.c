/* Needed to match: the library was built optimizing for size, without loop-invariant motion. */
#pragma optimize_for_size on
#pragma opt_loop_invariants off
#include "types.h"
#include "sdk/isd.h"

int Isd_DoubleSign(double x);
int Isd_DoubleExponent(double x);

/* Writes one byte with a halfword read and write, like Isd_ReadByte reads. */
static inline void Isd_StoreByte(char* address, int value) {
  if ((u32)address & 1) {
    *(u16*)(address - 1) =
        (*(u16*)(address - 1) & 0xff) | ((value & 0xff) << 8);
  } else {
    *(u16*)address = (*(u16*)address & 0xff00) | (value & 0xff);
  }
}

/*
 * Multiplies a double by 10 by hand for the formatter's %f output:
 * mantissa * 1.25 and exponent + 3, then renormalizes and packs.
 * The sign is shifted by 7 although Isd_DoubleSign already returns 0x80,
 * so it never reaches the result.
 */
double Isd_DoubleTimesTen(double x) {
  int exponent;
  int i;
  int k;
  int j;
  u64 mantissa;
  u64 shifted;
  double result;
  int byte;
  char* writePtr;
  char* readPtr;
  char* topByte;
  int sign;
  int count;

  sign = Isd_DoubleSign(x);
  exponent = Isd_DoubleExponent(x);
  mantissa = 0;
  topByte = (char*)&x + 6;
  mantissa |= 0x10;
  mantissa |= Isd_ReadByte(topByte) & 0xf;
  for (k = 5; k >= 0; k--) {
    mantissa <<= 8;
    mantissa |= Isd_ReadByte((char*)&x + k);
  }

  /* The implicit leading 1 at bit 52. */
  mantissa |= 0x0010000000000000ULL;
  exponent += 3;
  mantissa += mantissa >> 2;
  while ((mantissa >> 53) & 1) {
    mantissa >>= 1;
    exponent++;
  }

  result = 0;
  readPtr = (char*)&result + 7;
  byte = Isd_ReadByte(readPtr) | (sign << 7);
  writePtr = (char*)&result + 7;
  Isd_StoreByte(writePtr, byte);
  readPtr = (char*)&result + 7;
  byte = Isd_ReadByte(readPtr) | (exponent >> 4);
  writePtr = (char*)&result + 7;
  Isd_StoreByte(writePtr, byte);
  readPtr = (char*)&result + 6;
  /* Needed to match: the byte goes through i first, and the shift count below is a local. */
  i = Isd_ReadByte(readPtr) | ((exponent & 0xf) << 4);
  byte = i;
  writePtr = (char*)&result + 6;
  Isd_StoreByte(writePtr, byte);
  mantissa &= 0x000fffffffffffffULL;
  for (i = 6; i >= 0; i--) {
    shifted = mantissa;
    count = i * 8;
    for (j = 0; j < count; j++) {
      shifted >>= 1;
    }
    writePtr = (char*)&result + i;
    byte = (u8)shifted | Isd_ReadByte(writePtr);
    Isd_StoreByte(writePtr, byte);
  }
  return result;
}
