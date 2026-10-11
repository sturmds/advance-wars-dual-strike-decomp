#include "types.h"
#include "sdk/isd.h"

/* Returns the 11-bit exponent field of a double. */
int Isd_DoubleExponent(double x)
{
    char *topBytePtr = (char *)&x + 7;
    int topByte = Isd_ReadByte(topBytePtr);
    char *nextBytePtr = (char *)&x + 6;
    int nextByte = Isd_ReadByte(nextBytePtr);

    return ((topByte & 0x7f) << 4) + (nextByte >> 4);
}
