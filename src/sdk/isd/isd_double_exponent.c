#include "types.h"
#include "sdk/isd.h"

/* Returns the 11-bit exponent field of a double. */
int Isd_DoubleExponent(double x) {
  char* topBytePtr = &ISD_DOUBLE_BYTES(x)->signExponent;
  int topByte = Isd_ReadByte(topBytePtr);
  char* nextBytePtr = &ISD_DOUBLE_BYTES(x)->exponentLow;
  int nextByte = Isd_ReadByte(nextBytePtr);

  return ((topByte & 0x7f) << 4) + (nextByte >> 4);
}
