#include "types.h"
#include "sdk/msl.h"

/* Unsigned 64-bit division of the runtime (_ll_udiv); x % y on u64 calls _ull_mod. */
u64 func_02128f14(u64 dividend, u64 divisor);

char *longlong2str(long long value, char *bufferEnd, PrintFormat format)
{
    u64 magnitude;
    u64 base;
    char *p;
    int count;
    int digit;
    int minus;

    magnitude = value;
    minus = 0;
    count = 0;

    p = bufferEnd;
    *--p = '\0';

    if (value == 0 && format.precision == 0 && !(format.alternateForm && format.conversionChar == 'o')) {
        return p;
    }

    switch (format.conversionChar) {
    case 'd':
    case 'i':
        base = 10;
        if (value < 0) {
            magnitude = -magnitude;
            minus = 1;
        }
        break;
    case 'o':
        base = 8;
        format.signOptions = PRINT_SIGN_MINUS_ONLY;
        break;
    case 'u':
        base = 10;
        format.signOptions = PRINT_SIGN_MINUS_ONLY;
        break;
    case 'x':
    case 'X':
        base = 16;
        format.signOptions = PRINT_SIGN_MINUS_ONLY;
        break;
    }

    do {
        digit = magnitude % base;
        magnitude = func_02128f14(magnitude, base);
        if (digit < 10) {
            digit += '0';
        } else if (format.conversionChar == 'x') {
            digit += 'a' - 10;
        } else {
            digit += 'A' - 10;
        }
        *--p = digit;
        count++;
    } while (magnitude != 0);

    if (base == 8 && format.alternateForm && *p != '0') {
        *--p = '0';
        count++;
    }

    if (format.justification == PRINT_ZERO_FILL) {
        format.precision = format.fieldWidth;
        if (minus || format.signOptions != PRINT_SIGN_MINUS_ONLY) {
            format.precision--;
        }
        if (base == 16 && format.alternateForm) {
            format.precision -= 2;
        }
    }

    if (bufferEnd - p + format.precision > 509) {
        return NULL;
    }

    while (count < format.precision) {
        *--p = '0';
        count++;
    }

    if (base == 16 && format.alternateForm) {
        *--p = format.conversionChar;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.signOptions == PRINT_SIGN_ALWAYS) {
        *--p = '+';
    } else if (format.signOptions == PRINT_SIGN_SPACE) {
        *--p = ' ';
    }
    return p;
}
