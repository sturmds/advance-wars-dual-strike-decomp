/* Needed to match: the library was built optimizing for size, without loop-invariant motion. */
#pragma optimize_for_size on
#pragma opt_loop_invariants off
#include "types.h"
#include "sdk/isd.h"

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
 * Adds two non-negative doubles by hand for the formatter's %f output:
 * aligns the smaller mantissa to the larger exponent, adds, renormalizes
 * and packs the result. Zero (exponent and mantissa 0) returns the other
 * argument; signs are ignored.
 */
double func_02120574(double a, double b)
{
    int byte;
    int expA;
    int j;
    u64 mantA;
    int shift;
    char *topB;
    int k;
    char *readPtr;
    char *writePtr;
    u64 mantB;
    u64 shifted;
    char *topA;
    int expB;
    int i;
    double result;

    expA = Isd_DoubleExponent(a);
    mantA = 0;
    expB = Isd_DoubleExponent(b);
    mantB = 0;

    topA = (char *)&a + 6;
    mantA |= 0x10;
    mantA |= Isd_ReadByte(topA) & 0xf;
    for (k = 5; k >= 0; k--) {
        mantA <<= 8;
        mantA |= Isd_ReadByte((char *)&a + k);
    }
    topB = (char *)&b + 6;
    mantB |= 0x10;
    mantB |= Isd_ReadByte(topB) & 0xf;
    for (k = 5; k >= 0; k--) {
        mantB <<= 8;
        mantB |= Isd_ReadByte((char *)&b + k);
    }

    if (expA == 0 && mantA == 0) {
        return b;
    }
    if (expB == 0 && mantB == 0) {
        return a;
    }

    /* The implicit leading 1 at bit 52. */
    mantA |= 0x0010000000000000ULL;
    mantB |= 0x0010000000000000ULL;
    k = 0;
    if (expA >= expB) {
        shift = expA - expB;
        for (; k < shift; k++) {
            mantB >>= 1;
        }
        mantB = mantA + mantB;
    } else {
        shift = expB - expA;
        for (; k < shift; k++) {
            mantA >>= 1;
        }
        mantB = mantA + mantB;
        expA = expB;
    }
    /* A carry into bit 53 moves the point one place. */
    while ((mantB >> 53) & 1) {
        mantB >>= 1;
        expA++;
    }

    result = 0;
    readPtr = (char *)&result + 7;
    byte = Isd_ReadByte(readPtr) | (expA >> 4);
    writePtr = (char *)&result + 7;
    Isd_StoreByte(writePtr, byte);
    readPtr = (char *)&result + 6;
    byte = Isd_ReadByte(readPtr) | ((expA & 0xf) << 4);
    writePtr = (char *)&result + 6;
    Isd_StoreByte(writePtr, byte);
    mantB &= 0x000fffffffffffffULL;
    for (i = 6; i >= 0; i--) {
        shifted = mantB;
        for (j = 0; j < i * 8; j++) {
            shifted >>= 1;
        }
        writePtr = (char *)&result + i;
        byte = (u8)shifted | Isd_ReadByte(writePtr);
        Isd_StoreByte(writePtr, byte);
    }
    return result;
}
