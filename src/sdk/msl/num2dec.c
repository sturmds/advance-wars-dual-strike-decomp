#include "types.h"
#include "sdk/msl.h"

void __num2dec(const DecimalFormat *format, double x, Decimal *dec)
{
    s16 digits = format->digits;
    int i;

    func_02124c80(dec, x);
    /* Infinity and NaN come back as a letter, not a digit. */
    if (dec->digits[0] > 9) {
        return;
    }
    if (digits > DECIMAL_SIG_DIGITS) {
        digits = DECIMAL_SIG_DIGITS;
    }
    __rounddec(dec, digits);
    while (dec->length < digits) {
        dec->digits[dec->length++] = 0;
    }
    dec->exp -= dec->length - 1;
    for (i = 0; i < dec->length; i++) {
        dec->digits[i] += '0';
    }
}
