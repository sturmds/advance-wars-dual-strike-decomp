#include "types.h"
#include "sdk/msl.h"
#include "sdk/msl_string.h"

/* Declared with msl.h's Decimal: sdk/msl_decimal.h defines a second Decimal with the same layout. */
void __round_decimal(Decimal *dec, int newLength);

/* Character class table of the C locale; bit 0x200 marks upper case letters. */
extern const u16 data_02151b68[];

static inline int IsUpper(int c)
{
    return (c < 0 || c >= 128) ? 0 : (data_02151b68[c] & 0x200);
}

extern const char data_0216df00[]; /* "-INF" */
extern const char data_0216df08[]; /* "-inf" */
extern const char data_0216df10[]; /* "INF" */
extern const char data_0216df14[]; /* "inf" */
extern const char data_0216df18[]; /* "-NAN" */
extern const char data_0216df20[]; /* "-nan" */
extern const char data_0216df28[]; /* "NAN" */
extern const char data_0216df2c[]; /* "nan" */

/* Writes value backwards ending at bufferEnd for %e, %f and %g; returns the start of the text, or NULL if it is too long. */
char *float2str(long double value, char *bufferEnd, PrintFormat format)
{
    Decimal dec;
    DecimalFormat form;
    char *p;
    char *q;
    int n;
    int digits;
    int sign;
    int intDigits;
    int fracDigits;

    form.style = 0;
    form.digits = DECIMAL_SIG_DIGITS;
    __num2dec(&form, value, &dec);

    /* Drop trailing zeros of the digit string. */
    p = (char *)dec.digits + dec.length;
    while (dec.length > 1 && *--p == '0') {
        dec.length--;
        dec.exp++;
    }

    switch (dec.digits[0]) {
    case '0':
        dec.exp = 0;
        break;

    case 'I':
        if (value < 0) {
            p = bufferEnd - 5;
            if (IsUpper(format.conversionChar)) {
                strcpy(p, data_0216df00);
            } else {
                strcpy(p, data_0216df08);
            }
        } else {
            p = bufferEnd - 4;
            if (IsUpper(format.conversionChar)) {
                strcpy(p, data_0216df10);
            } else {
                strcpy(p, data_0216df14);
            }
        }
        return p;

    case 'N':
        if (dec.sign) {
            p = bufferEnd - 5;
            if (IsUpper(format.conversionChar)) {
                strcpy(p, data_0216df18);
            } else {
                strcpy(p, data_0216df20);
            }
        } else {
            p = bufferEnd - 4;
            if (IsUpper(format.conversionChar)) {
                strcpy(p, data_0216df28);
            } else {
                strcpy(p, data_0216df2c);
            }
        }
        return p;
    }

    dec.exp += dec.length - 1;

    p = bufferEnd;
    *--p = 0;

    switch (format.conversionChar) {
    case 'g':
    case 'G':
        if (dec.length > format.precision) {
            __round_decimal(&dec, format.precision);
        }

        if (dec.exp < -4 || dec.exp >= format.precision) {
            if (format.alternateForm) {
                format.precision--;
            } else {
                format.precision = dec.length - 1;
            }
            if (format.conversionChar == 'g') {
                format.conversionChar = 'e';
            } else {
                format.conversionChar = 'E';
            }
            goto e_format;
        }

        if (format.alternateForm) {
            format.precision -= dec.exp + 1;
        } else if ((format.precision = dec.length - (dec.exp + 1)) < 0) {
            format.precision = 0;
        }
        goto f_format;

    case 'e':
    case 'E':
    e_format:
        if (dec.length > format.precision + 1) {
            __round_decimal(&dec, format.precision + 1);
        }

        n = dec.exp;
        sign = '+';
        if (n < 0) {
            n = -n;
            sign = '-';
        }

        for (digits = 0; n || digits < 2; digits++) {
            *--p = n % 10 + '0';
            n /= 10;
        }

        *--p = sign;
        *--p = format.conversionChar;

        if (bufferEnd - p + format.precision > PRINT_CONVERSION_MAX) {
            return NULL;
        }

        if (dec.length < format.precision + 1) {
            for (n = format.precision + 2 - dec.length; --n;) {
                *--p = '0';
            }
        }

        for (n = dec.length, q = (char *)dec.digits + dec.length; --n;) {
            *--p = *--q;
        }

        if (format.precision || format.alternateForm) {
            *--p = '.';
        }

        *--p = dec.digits[0];

        if (dec.sign) {
            *--p = '-';
        } else if (format.signOptions == PRINT_SIGN_ALWAYS) {
            *--p = '+';
        } else if (format.signOptions == PRINT_SIGN_SPACE) {
            *--p = ' ';
        }
        break;

    case 'f':
    case 'F':
    f_format:
        if ((fracDigits = dec.length - dec.exp - 1) < 0) {
            fracDigits = 0;
        }

        if (fracDigits > format.precision) {
            __round_decimal(&dec, dec.length - (fracDigits - format.precision));
            if ((fracDigits = dec.length - dec.exp - 1) < 0) {
                fracDigits = 0;
            }
        }

        if ((intDigits = dec.exp + 1) < 0) {
            intDigits = 0;
        }

        if (intDigits + fracDigits > PRINT_CONVERSION_MAX) {
            return NULL;
        }

        q = (char *)dec.digits + dec.length;

        for (digits = 0; digits < format.precision - fracDigits; digits++) {
            *--p = '0';
        }
        for (digits = 0; digits < fracDigits && digits < dec.length; digits++) {
            *--p = *--q;
        }
        for (; digits < fracDigits; digits++) {
            *--p = '0';
        }

        if (format.precision || format.alternateForm) {
            *--p = '.';
        }

        if (intDigits) {
            for (digits = 0; digits < intDigits - dec.length; digits++) {
                *--p = '0';
            }
            for (; digits < intDigits; digits++) {
                *--p = *--q;
            }
        } else {
            *--p = '0';
        }

        if (dec.sign) {
            *--p = '-';
        } else if (format.signOptions == PRINT_SIGN_ALWAYS) {
            *--p = '+';
        } else if (format.signOptions == PRINT_SIGN_SPACE) {
            *--p = ' ';
        }
        break;
    }

    return p;
}
