#ifndef SDK_MSL_H
#define SDK_MSL_H

#include "stdarg.h"
#include "types.h"

/*
 * Floating-point trap interface of the runtime. Wrappers around the soft-float
 * routines pass their operands and an operation word to one dispatcher. The
 * word holds the operation in bits 26-30 and the rounding mode in bits 22-23.
 * The dispatcher calls the handler registered for the operation.
 */
typedef union FpValue {
    u32 word[2];
    float f;
    double d;
    s32 i;
} FpValue;

#define FP_TRAP_OP_MASK 0x7C000000
#define FP_TRAP_ROUNDING_MASK 0x00C00000

typedef FpValue (*FpTrapHandler)(FpValue x, FpValue y, u32 op);

/* Returns a pointer to the first byte equal to ch in the first count bytes of buf, or NULL. */
void *memchr(const void *buf, int ch, u32 count);
/* Copies count bytes from the start; returns dest. */
void *__copy_mem(void *dest, const void *src, u32 count);

/* Destination of vsnprintf: the characters that fit in capacity are stored, the rest are dropped. */
typedef struct StringSink {
    char *buffer;
    u32 capacity;
    u32 length;
} StringSink;

typedef int (*FormatWriteProc)(void *sink, const char *src, u32 count);

u32 strlen(const char *str);

/* Doubles pass in a register pair; the sign is bit 31 of the high word. */
double fabs(double x);
double copysign(double x, double y);
/* Returns the fraction of x in [0.5, 1) and stores the power of two in *exponent. */
double frexp(double x, int *exponent);
/* Returns x * 2^n. */
double ldexp(double x, int n);

/* Classes returned by __fpclassifyd. */
#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5

static inline int __fpclassifyd(double x)
{
    switch (((int *)&x)[1] & 0x7FF00000) {
    case 0x7FF00000:
        if ((((int *)&x)[1] & 0x000FFFFF) || ((int *)&x)[0]) {
            return FP_NAN;
        }
        return FP_INFINITE;
    case 0:
        if ((((int *)&x)[1] & 0x000FFFFF) || ((int *)&x)[0]) {
            return FP_SUBNORMAL;
        }
        return FP_ZERO;
    }
    return FP_NORMAL;
}

/* Digit string of a float conversion with its decimal exponent. */
typedef struct Decimal {
    u8 sign;
    u8 unk_01;
    s16 exp;
    u8 length;
    u8 digits[32]; /* 38 bytes in all: the stack frames of __num2dec_internal show it */
    u8 pad_25;
} Decimal;

/* How many digits a float conversion asks for. */
typedef struct DecimalFormat {
    u8 style;
    u8 pad_01;
    s16 digits;
} DecimalFormat;

#define DECIMAL_SIG_DIGITS 32

/* Converts x to at most format->digits significant digits, as ASCII. */
void __num2dec(const DecimalFormat *format, double x, Decimal *dec);
/* Converts x to its exact digit string, digits as values 0-9. */
void __num2dec_internal(Decimal *dec, double x);
/* Sets dec to 2^exponent. */
void __two_exp(Decimal *dec, long exponent);
/* Sets dec to the ASCII digit string times 10^exponent; digits past the 32nd round it up. */
void __str2dec(Decimal *dec, const char *digits, s16 exponent);
/* Sets result to x * y. */
void __timesdec(Decimal *result, const Decimal *x, const Decimal *y);
/* Sets dec to the digits of value. */
void __ull2dec(Decimal *dec, u64 value);

/* Adds one to the digit string at its first count digits; a carry out of the first digit raises exp. */
void func_02125658(Decimal *dec, int count);
/* Returns 1 to round the digits at count up, -1 to round down; an exact half goes to the even digit. */
int __must_round(Decimal *dec, int count);
/* Rounds the digit string to count digits. */
void __rounddec(Decimal *dec, int count);
/* Returns the number of trailing zero bits of value, 32 for 0. */
int __cnt_trail_zeros(u32 value);

int func_02122508(FormatWriteProc write, void *sink, const char *format, va_list args);
int __StringWrite(void *context, const char *src, u32 count);
int vsnprintf(char *buf, u32 size, const char *format, va_list args);
int snprintf(char *buf, u32 size, const char *format, ...);
int sprintf(char *buf, const char *format, ...);

#endif
