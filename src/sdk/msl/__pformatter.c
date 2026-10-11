#include "stdarg.h"
#include "types.h"
#include "sdk/msl.h"
#include "sdk/msl_string.h"
#include "sdk/msl_wchar.h"

#define CONVERSION_BUFFER_SIZE 512

char *func_02122d44(long double value, char *bufferEnd, PrintFormat format);

extern const wchar_t data_0216def8[]; /* L"" */
extern const char data_0216defc[];    /* "" */

/*
 * Printf core: copies format to write() and expands each conversion.
 *
 * Not matched yet. The original passes PrintFormat by value by storing it
 * just below sp into a 12-byte argument area kept at the bottom of its frame.
 * mwccarm 2.0 (every build in metroskrew) moves sp around each such call
 * instead, which needs a frame pointer in r11 and shifts every stack offset.
 * The control flow and the rest of the code already line up.
 */
int __pformatter(FormatWriteProc write, void *sink, const char *format, va_list args)
{
    int count;
    int written;
    int width;
    const char *cursor;
    const char *conversionStart;
    PrintFormat spec;
    long value;
    long long longValue;
    long double doubleValue;
    char buffer[CONVERSION_BUFFER_SIZE];
    char *text;
    char *end;
    char fillChar = ' ';

    cursor = format;
    written = 0;

    while (*cursor) {
        if (!(conversionStart = strchr(cursor, '%'))) {
            count = strlen(cursor);
            written += count;
            if (count && !write(sink, cursor, count)) {
                return -1;
            }
            break;
        }

        count = conversionStart - cursor;
        written += count;
        if (count && !write(sink, cursor, count)) {
            return -1;
        }

        cursor = conversionStart;
        cursor = parse_format(cursor, &args, &spec);

        switch (spec.conversionChar) {
        case 'd':
        case 'i':
            if (spec.argumentOptions == PRINT_ARG_LONG) {
                value = va_arg(args, long);
            } else if (spec.argumentOptions == PRINT_ARG_LONG_LONG) {
                longValue = va_arg(args, long long);
            } else if (spec.argumentOptions == PRINT_ARG_INTMAX) {
                value = va_arg(args, long long);
            } else if (spec.argumentOptions == PRINT_ARG_SIZE) {
                value = va_arg(args, u32);
            } else if (spec.argumentOptions == PRINT_ARG_PTRDIFF) {
                value = va_arg(args, long);
            } else {
                value = va_arg(args, int);
            }

            if (spec.argumentOptions == PRINT_ARG_SHORT) {
                value = (short)value;
            }
            if (spec.argumentOptions == PRINT_ARG_CHAR) {
                value = (signed char)value;
            }

            if (spec.argumentOptions == PRINT_ARG_LONG_LONG) {
                if (!(text = longlong2str(longValue, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                    goto conversion_error;
                }
            } else {
                if (!(text = long2str(value, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                    goto conversion_error;
                }
            }
            count = buffer + CONVERSION_BUFFER_SIZE - 1 - text;
            break;

        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (spec.argumentOptions == PRINT_ARG_LONG) {
                value = va_arg(args, unsigned long);
            } else if (spec.argumentOptions == PRINT_ARG_LONG_LONG) {
                longValue = va_arg(args, long long);
            } else if (spec.argumentOptions == PRINT_ARG_INTMAX) {
                value = va_arg(args, long long);
            } else if (spec.argumentOptions == PRINT_ARG_SIZE) {
                value = va_arg(args, u32);
            } else if (spec.argumentOptions == PRINT_ARG_PTRDIFF) {
                value = va_arg(args, long);
            } else {
                value = va_arg(args, unsigned int);
            }

            if (spec.argumentOptions == PRINT_ARG_SHORT) {
                value = (unsigned short)value;
            }
            if (spec.argumentOptions == PRINT_ARG_CHAR) {
                value = (unsigned char)value;
            }

            if (spec.argumentOptions == PRINT_ARG_LONG_LONG) {
                if (!(text = longlong2str(longValue, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                    goto conversion_error;
                }
            } else {
                if (!(text = long2str(value, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                    goto conversion_error;
                }
            }
            count = buffer + CONVERSION_BUFFER_SIZE - 1 - text;
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (spec.argumentOptions == PRINT_ARG_LONG_DOUBLE) {
                doubleValue = va_arg(args, long double);
            } else {
                doubleValue = va_arg(args, double);
            }
            if (!(text = func_02122d44(doubleValue, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                goto conversion_error;
            }
            count = buffer + CONVERSION_BUFFER_SIZE - 1 - text;
            break;

        case 'a':
        case 'A':
            if (spec.argumentOptions == PRINT_ARG_LONG_DOUBLE) {
                doubleValue = va_arg(args, long double);
            } else {
                doubleValue = va_arg(args, double);
            }
            if (!(text = double2hex(doubleValue, buffer + CONVERSION_BUFFER_SIZE, spec))) {
                goto conversion_error;
            }
            count = buffer + CONVERSION_BUFFER_SIZE - 1 - text;
            break;

        case 's':
            if (spec.argumentOptions == PRINT_ARG_WCHAR) {
                const wchar_t *wide = va_arg(args, const wchar_t *);
                if (wide == NULL) {
                    wide = data_0216def8;
                }
                if ((count = wcstombs(buffer, wide, sizeof(buffer))) < 0) {
                    goto conversion_error;
                }
                text = buffer;
            } else {
                text = va_arg(args, char *);
            }

            if (text == NULL) {
                text = (char *)data_0216defc;
            }

            if (spec.alternateForm) {
                /* "%#s" prints a Pascal string: the first byte holds the length. */
                count = *(u8 *)text++;
                if (spec.precisionSpecified && count > spec.precision) {
                    count = spec.precision;
                }
            } else if (spec.precisionSpecified) {
                count = spec.precision;
                if ((end = memchr(text, 0, count)) != NULL) {
                    count = end - text;
                }
            } else {
                count = strlen(text);
            }
            break;

        case 'n':
            text = va_arg(args, char *);
            switch (spec.argumentOptions) {
            case PRINT_ARG_NORMAL:
                *(int *)text = written;
                break;
            case PRINT_ARG_SHORT:
                *(short *)text = written;
                break;
            case PRINT_ARG_LONG:
                *(long *)text = written;
                break;
            case PRINT_ARG_LONG_LONG:
                *(long long *)text = written;
                break;
            }
            continue;

        case 'c':
            text = buffer;
            *text = va_arg(args, int);
            count = 1;
            break;

        case '%':
            text = buffer;
            *text = '%';
            count = 1;
            break;

        case PRINT_BAD_CONVERSION:
        default:
        conversion_error:
            count = strlen(conversionStart);
            written += count;
            if (count && !write(sink, conversionStart, count)) {
                return -1;
            }
            return written;
        }

        width = count;

        if (spec.justification != PRINT_JUSTIFY_LEFT) {
            fillChar = (spec.justification == PRINT_ZERO_FILL) ? '0' : ' ';

            /* Zero padding goes after the sign. */
            if ((*text == '+' || *text == '-' || *text == ' ') && fillChar == '0') {
                if (write(sink, text, 1) == 0) {
                    return -1;
                }
                text++;
                count--;
            }

            while (width < spec.fieldWidth) {
                if (write(sink, &fillChar, 1) == 0) {
                    return -1;
                }
                width++;
            }
        }

        if (count && !write(sink, text, count)) {
            return -1;
        }

        if (spec.justification == PRINT_JUSTIFY_LEFT) {
            while (width < spec.fieldWidth) {
                char blank = ' ';
                if (write(sink, &blank, 1) == 0) {
                    return -1;
                }
                width++;
            }
        }

        written += width;
    }

    return written;
}
