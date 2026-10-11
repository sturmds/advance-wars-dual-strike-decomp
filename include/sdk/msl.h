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

int func_02122508(FormatWriteProc write, void *sink, const char *format, va_list args);
int __StringWrite(void *context, const char *src, u32 count);
int vsnprintf(char *buf, u32 size, const char *format, va_list args);
int snprintf(char *buf, u32 size, const char *format, ...);
int sprintf(char *buf, const char *format, ...);

#endif
