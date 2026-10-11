#ifndef SDK_MSL_DECIMAL_H
#define SDK_MSL_DECIMAL_H

#include "types.h"

#define DECIMAL_MAX_DIGITS 36

/* A number as ASCII digits, used by the printf and scanf float conversions. */
typedef struct Decimal {
    u8 sign;
    u8 pad_01;
    s16 exponent;
    u8 length;                         /* digits in use */
    u8 digits[DECIMAL_MAX_DIGITS + 1]; /* ASCII '0'-'9' */
} Decimal;

/* Rounds to newLength digits, half to even. A negative newLength gives zero. */
void __round_decimal(Decimal *dec, int newLength);

#endif
