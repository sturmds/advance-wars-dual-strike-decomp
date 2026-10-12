/* Needed to match: the library was built optimizing for size, without loop-invariant motion. */
#pragma optimize_for_size on
#pragma opt_loop_invariants off
#include "types.h"
#include "sdk/isd.h"

int Isd_DoubleSign(double x);
int Isd_DoubleExponent(double x);

/* Writes one byte with a halfword read and write, like Isd_ReadByte reads. */
static inline void Isd_StoreByte(char *address, int value)
{
    if ((u32)address & 1) {
        *(u16 *)(address - 1) = (*(u16 *)(address - 1) & 0xff) | ((value & 0xff) << 8);
    } else {
        *(u16 *)address = (*(u16 *)address & 0xff00) | (value & 0xff);
    }
}

/*
 * Divides a double by 10 by hand for the formatter's %f output: x / 10 =
 * (mantissa - mantissa / 5) / 8. With keepFraction 0 it works only on the
 * integer bits and drops the fraction of the quotient.
 */
double func_0211fe0c(double x, int keepFraction)
{
    u64 mantissa;
    int k;
    int count;
    u64 fifth;
    int integerBits;
    char *writePtr;
    u32 remainder;
    int exponent;
    int sign;
    int j;
    char *readPtr;
    u64 shifted;
    BOOL integerOnly;
    char *topByte;
    int byte;
    int i;
    u64 fraction;
    double result;

    sign = Isd_DoubleSign(x);
    exponent = Isd_DoubleExponent(x);
    mantissa = 0;
    topByte = (char *)&x + 6;
    mantissa |= 0x10;
    mantissa |= Isd_ReadByte(topByte) & 0xf;
    for (k = 5; k >= 0; k--) {
        mantissa <<= 8;
        mantissa |= Isd_ReadByte((char *)&x + k);
    }

    integerBits = exponent - 1021;
    if (exponent >= 3) {
        exponent -= 3;
    } else {
        exponent = 0;
    }
    integerOnly = FALSE;
    if (keepFraction == 0) {
        integerOnly = TRUE;
    }
    fifth = 0;
    remainder = 0;
    mantissa |= 0x0010000000000000ULL;
    /* Long division of the mantissa bits by 5, top bit first; the remainder rounds the fifth up. */
    for (i = 0; i < ((integerBits <= 53 && integerOnly) ? integerBits : 53); i++) {
        fifth <<= 1;
        remainder <<= 1;
        shifted = mantissa;
        count = 52 - i;
        for (j = 0; j < count; j++) {
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
    count = 53 - i;
    for (j = 0; j < count; j++) {
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
        count = 53 - integerBits;
        for (j = 0; j < count; j++) {
            mantissa >>= 1;
        }
        for (j = 0; j < count; j++) {
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
    readPtr = (char *)&result + 7;
    byte = Isd_ReadByte(readPtr) | (sign << 7);
    writePtr = (char *)&result + 7;
    Isd_StoreByte(writePtr, byte);
    readPtr = (char *)&result + 7;
    byte = Isd_ReadByte(readPtr) | (exponent >> 4);
    writePtr = (char *)&result + 7;
    Isd_StoreByte(writePtr, byte);
    readPtr = (char *)&result + 6;
    byte = Isd_ReadByte(readPtr) | ((exponent & 0xf) << 4);
    writePtr = (char *)&result + 6;
    Isd_StoreByte(writePtr, byte);
    for (i = 6; i >= 0; i--) {
        shifted = fraction;
        count = i * 8;
        for (j = 0; j < count; j++) {
            shifted >>= 1;
        }
        writePtr = (char *)&result + i;
        byte = (u8)shifted | Isd_ReadByte(writePtr);
        Isd_StoreByte(writePtr, byte);
    }
    return result;
}
