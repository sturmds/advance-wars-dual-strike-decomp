/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

int Isd_DoubleExponent(double x);

/*
 * Returns the last decimal digit of the integer part of a non-negative double,
 * for the formatter's %f output. It walks the integer bits of the mantissa from
 * the top, doubling a running remainder mod 10.
 */
u32 Isd_DoubleUnitsDigit(double x) {
  /* Needed to match: this declaration order. */
  u64 mantissa;
  u32 digit;
  int j;
  int integerBits;
  u64 shifted;
  char* topMantissaByte;
  int exponent;
  int bit;
  int i;

  exponent = Isd_DoubleExponent(x);

  /* The 52 stored mantissa bits with the implicit leading 1 above them. */
  mantissa = 0;
  topMantissaByte = (char*)&x + 6;
  mantissa |= 0x10;
  mantissa |= Isd_ReadByte(topMantissaByte) & 0xf;
  for (i = 5; i >= 0; i--) {
    mantissa <<= 8;
    mantissa |= Isd_ReadByte((char*)&x + i);
  }

  /* Biased exponent 1023 means one integer bit. */
  integerBits = exponent - 1022;
  digit = 0;
  for (bit = 0; bit < (integerBits <= 53 ? integerBits : 53); bit++) {
    digit <<= 1;
    shifted = mantissa;
    for (j = 0; j < 52 - bit; j++) {
      shifted >>= 1;
    }
    if (shifted & 1) {
      digit++;
    }
    if (digit >= 10) {
      digit -= 10;
    }
  }
  return digit;
}
