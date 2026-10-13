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
 * Divides a double by 10 by hand for the formatter's %f output: x / 10 =
 * (mantissa - mantissa / 5) / 8. With keepFraction 0 it works only on the
 * integer bits and drops the fraction of the quotient.
 */
double Isd_DoubleDivideByTen(double x, int keepFraction) {
  u64 mantissa;
  int k;
  /* Needed to match: one count variable per shift loop. */
  int count;
  int padBits;
  int fractionBits;
  int shiftBits;
  u64 fifth;
  int integerBits;
  char* writePtr;
  u32 remainder;
  int exponent;
  int sign;
  int j;
  char* readPtr;
  u64 shifted;
  u32 integerOnly;
  char* bytePtr;
  int byte;
  int i;
  u64 fraction;
  double result;

  sign = Isd_DoubleSign(x);
  exponent = Isd_DoubleExponent(x);
  mantissa = 0;
  bytePtr = (char*)&x + 6;
  mantissa |= 0x10;
  mantissa |= Isd_ReadByte(bytePtr) & 0xf;
  for (k = 5; k >= 0; k--) {
    mantissa <<= 8;
    mantissa |= Isd_ReadByte((char*)&x + k);
  }

  integerBits = exponent - 1021;
  if (exponent >= 3) {
    exponent -= 3;
  } else {
    exponent = 0;
  }
  mantissa |= 0x0010000000000000ULL;
  fifth = 0;
  remainder = 0;
  i = 0;
  integerOnly = keepFraction == 0 ? TRUE : FALSE;
  /* Long division of the mantissa bits by 5, top bit first; the remainder rounds the fifth up. */
  for (; i < ((integerBits <= 53 && integerOnly) ? integerBits : 53); i++) {
    fifth <<= 1;
    remainder <<= 1;
    shifted = mantissa;
    j = 0;
    count = 52 - i;
    for (; j < count; j++) {
      shifted >>= 1;
    }
    if (shifted & 1) {
      remainder++;
    }
    if (remainder >= 5) {
      fifth += 1;
      remainder -= 5;
    }
  }
  if (remainder != 0) {
    fifth += 1;
  }
  padBits = 53 - i;
  for (j = 0; j < padBits; j++) {
    fifth <<= 1;
  }

  if (integerBits >= 3) {
    integerBits -= 3;
  } else {
    integerBits = 0;
  }
  mantissa -= fifth;
  /* Clear the bits below the point. */
  if (!keepFraction) {
    fractionBits = 53 - integerBits;
    for (j = 0; j < fractionBits; j++) {
      mantissa >>= 1;
    }
    for (j = 0; j < fractionBits; j++) {
      mantissa <<= 1;
    }
  }
  while (!((mantissa >> 52) & 1) && mantissa != 0) {
    exponent--;
    mantissa <<= 1;
  }

  fraction = mantissa & 0x000fffffffffffffULL;
  if (fraction == 0 && exponent < 0x3ff && !keepFraction) {
    sign = 0;
    exponent = 0;
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
  byte = Isd_ReadByte(readPtr) | ((exponent & 0xf) << 4);
  writePtr = (char*)&result + 6;
  Isd_StoreByte(writePtr, byte);
  /* Needed to match: this loop keeps its bits in fifth and its pointer in bytePtr. */
  for (i = 6; i >= 0; i--) {
    fifth = fraction;
    shiftBits = i * 8;
    for (j = 0; j < shiftBits; j++) {
      fifth >>= 1;
    }
    bytePtr = (char*)&result + i;
    byte = (u8)fifth | Isd_ReadByte(bytePtr);
    Isd_StoreByte(bytePtr, byte);
  }
  return result;
}
