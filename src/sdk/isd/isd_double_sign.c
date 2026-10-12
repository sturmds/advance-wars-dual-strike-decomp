#include "types.h"
#include "sdk/isd.h"

/* Returns 0x80 when the double is negative (the sign bit of its top byte), else 0. */
int Isd_DoubleSign(double x) {
  char* topByte = (char*)&x + 7;

  return Isd_ReadByte(topByte) & 0x80;
}
