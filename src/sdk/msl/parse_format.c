#include "types.h"
#include "stdarg.h"
#include "sdk/msl.h"

/* __ctype_map bit 3 marks the digits. */
#define IS_DIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (__ctype_map[c] & 8))

const char *parse_format(const char *format, va_list *args, PrintFormat *out)
{
    PrintFormat f;
    const char *s;
    int c;
    int flagFound;

    s = format;
    f.justification = PRINT_JUSTIFY_RIGHT;
    f.signOptions = PRINT_SIGN_MINUS_ONLY;
    f.precisionSpecified = 0;
    f.alternateForm = 0;
    f.argumentOptions = PRINT_ARG_NORMAL;
    f.fieldWidth = 0;
    f.precision = 0;

    if ((c = *++s) == '%') {
        f.conversionChar = c;
        *out = f;
        return s + 1;
    }

    for (;;) {
        flagFound = 1;
        switch (c) {
        case '-':
            f.justification = PRINT_JUSTIFY_LEFT;
            break;
        case '+':
            f.signOptions = PRINT_SIGN_ALWAYS;
            break;
        case ' ':
            if (f.signOptions != PRINT_SIGN_ALWAYS) {
                f.signOptions = PRINT_SIGN_SPACE;
            }
            break;
        case '#':
            f.alternateForm = 1;
            break;
        case '0':
            if (f.justification != PRINT_JUSTIFY_LEFT) {
                f.justification = PRINT_ZERO_FILL;
            }
            break;
        default:
            flagFound = 0;
        }
        if (flagFound) {
            c = *++s;
        } else {
            break;
        }
    }

    if (c == '*') {
        if ((f.fieldWidth = va_arg(*args, int)) < 0) {
            f.justification = PRINT_JUSTIFY_LEFT;
            f.fieldWidth = -f.fieldWidth;
        }
        c = *++s;
    } else {
        while (IS_DIGIT(c)) {
            f.fieldWidth = f.fieldWidth * 10 + (c - '0');
            c = *++s;
        }
    }

    if (f.fieldWidth > PRINT_CONVERSION_MAX) {
        f.conversionChar = PRINT_BAD_CONVERSION;
        *out = f;
        return s + 1;
    }

    if (c == '.') {
        f.precisionSpecified = 1;
        if ((c = *++s) == '*') {
            if ((f.precision = va_arg(*args, int)) < 0) {
                f.precisionSpecified = 0;
            }
            c = *++s;
        } else {
            while (IS_DIGIT(c)) {
                f.precision = f.precision * 10 + (c - '0');
                c = *++s;
            }
        }
    }

    flagFound = 1;
    switch (c) {
    case 'h':
        f.argumentOptions = PRINT_ARG_SHORT;
        if (s[1] == 'h') {
            f.argumentOptions = PRINT_ARG_CHAR;
            c = *++s;
        }
        break;
    case 'l':
        f.argumentOptions = PRINT_ARG_LONG;
        if (s[1] == 'l') {
            f.argumentOptions = PRINT_ARG_LONG_LONG;
            c = *++s;
        }
        break;
    case 'L':
        f.argumentOptions = PRINT_ARG_LONG_DOUBLE;
        break;
    case 'j':
        f.argumentOptions = PRINT_ARG_INTMAX;
        break;
    case 'z':
        f.argumentOptions = PRINT_ARG_SIZE;
        break;
    case 't':
        f.argumentOptions = PRINT_ARG_PTRDIFF;
        break;
    default:
        flagFound = 0;
    }
    if (flagFound) {
        c = *++s;
    }

    f.conversionChar = c;

    switch (c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argumentOptions == PRINT_ARG_LONG_DOUBLE) {
            f.conversionChar = PRINT_BAD_CONVERSION;
            break;
        }
        if (!f.precisionSpecified) {
            f.precision = 1;
        } else if (f.justification == PRINT_ZERO_FILL) {
            f.justification = PRINT_JUSTIFY_RIGHT;
        }
        break;

    case 'f':
    case 'F':
        if (f.argumentOptions == PRINT_ARG_SHORT || f.argumentOptions == PRINT_ARG_INTMAX ||
            f.argumentOptions == PRINT_ARG_SIZE || f.argumentOptions == PRINT_ARG_PTRDIFF ||
            f.argumentOptions == PRINT_ARG_LONG_LONG) {
            f.conversionChar = PRINT_BAD_CONVERSION;
            break;
        }
        if (!f.precisionSpecified) {
            f.precision = 6;
        }
        break;

    case 'a':
    case 'A':
        /* 13 hex digits hold the 52 stored mantissa bits of a double. */
        if (!f.precisionSpecified) {
            f.precision = 13;
        }
        if (f.argumentOptions == PRINT_ARG_SHORT || f.argumentOptions == PRINT_ARG_INTMAX ||
            f.argumentOptions == PRINT_ARG_SIZE || f.argumentOptions == PRINT_ARG_PTRDIFF ||
            f.argumentOptions == PRINT_ARG_LONG_LONG || f.argumentOptions == PRINT_ARG_CHAR) {
            f.conversionChar = PRINT_BAD_CONVERSION;
        }
        break;

    case 'g':
    case 'G':
        if (f.precision == 0) {
            f.precision = 1;
        }
        /* fall through */
    case 'e':
    case 'E':
        if (f.argumentOptions == PRINT_ARG_SHORT || f.argumentOptions == PRINT_ARG_INTMAX ||
            f.argumentOptions == PRINT_ARG_SIZE || f.argumentOptions == PRINT_ARG_PTRDIFF ||
            f.argumentOptions == PRINT_ARG_LONG_LONG || f.argumentOptions == PRINT_ARG_CHAR) {
            f.conversionChar = PRINT_BAD_CONVERSION;
            break;
        }
        if (!f.precisionSpecified) {
            f.precision = 6;
        }
        break;

    case 'p':
        f.conversionChar = 'x';
        f.alternateForm = 1;
        f.argumentOptions = PRINT_ARG_LONG;
        f.precision = 8;
        break;

    case 'c':
        if (f.argumentOptions == PRINT_ARG_LONG) {
            f.argumentOptions = PRINT_ARG_WCHAR;
        } else if (f.precisionSpecified || f.argumentOptions != PRINT_ARG_NORMAL) {
            f.conversionChar = PRINT_BAD_CONVERSION;
        }
        break;

    case 's':
        if (f.argumentOptions == PRINT_ARG_LONG) {
            f.argumentOptions = PRINT_ARG_WCHAR;
        } else if (f.argumentOptions != PRINT_ARG_NORMAL) {
            f.conversionChar = PRINT_BAD_CONVERSION;
        }
        break;

    case 'n':
        if (f.argumentOptions == PRINT_ARG_LONG_DOUBLE) {
            f.conversionChar = PRINT_BAD_CONVERSION;
        }
        break;

    default:
        f.conversionChar = PRINT_BAD_CONVERSION;
        break;
    }

    *out = f;
    return s + 1;
}
