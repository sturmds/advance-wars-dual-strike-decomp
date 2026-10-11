#include "stdarg.h"
#include "types.h"
#include "sdk/msl.h"

int snprintf(char *buf, u32 size, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    return vsnprintf(buf, size, format, args);
}
