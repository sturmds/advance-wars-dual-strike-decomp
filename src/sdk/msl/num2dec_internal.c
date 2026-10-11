#include "types.h"
#include "sdk/msl.h"

/* Soft-float runtime: double subtract, and double to unsigned 64-bit. */
double func_0212676c(double a, double b);
u64 func_02125e7c(double x);

/* Number of trailing zero bits in the 53-bit mantissa of x. */
static inline int CountTrailingZeros(double x)
{
    u32 *words = (u32 *)&x;

    if (words[0] != 0) {
        return __cnt_trail_zeros(words[0]);
    }
    return 32 + __cnt_trail_zeros(words[1] | 0x00100000);
}

void __num2dec_internal(Decimal *dec, double x)
{
    u8 sign = (((int *)&x)[1] & 0x80000000) != 0;

    if (x == 0.0) {
        dec->sign = sign;
        dec->exp = 0;
        dec->length = 1;
        dec->digits[0] = 0;
        return;
    }
    if (__fpclassifyd(x) <= FP_INFINITE) {
        dec->sign = sign;
        dec->exp = 0;
        dec->length = 1;
        dec->digits[0] = __fpclassifyd(x) == FP_NAN ? 'N' : 'I';
        return;
    }
    if (sign) {
        x = func_0212676c(0.0, x);
    }
    /* x = mantissa * 2^(exponent - bits), with the mantissa as an exact integer. */
    {
        int exponent;
        double fraction = frexp(x, &exponent);
        int bits = 53 - CountTrailingZeros(fraction);
        Decimal mantissa;
        Decimal power;

        __two_exp(&power, exponent - bits);
        __ull2dec(&mantissa, func_02125e7c(ldexp(fraction, bits)));
        __timesdec(dec, &mantissa, &power);
        dec->sign = sign;
    }
}
