#include "types.h"
#include "sdk/msl.h"

/* Unsigned 64-bit divide of the runtime. Writing `/` would call it as _ll_udiv, a name it does not have yet. */
u64 func_02128f14(u64 dividend, u64 divisor);

void __ull2dec(Decimal *dec, u64 value)
{
    dec->sign = 0;
    dec->length = 0;
    /* Digits come out lowest first, then get reversed. */
    for (; value != 0; value = func_02128f14(value, 10)) {
        dec->digits[dec->length++] = value % 10;
    }
    {
        u8 *front = dec->digits;
        u8 *back = dec->digits + dec->length;

        for (; front < --back; front++) {
            u8 digit = *front;

            *front = *back;
            *back = digit;
        }
    }
    dec->exp = dec->length - 1;
}
