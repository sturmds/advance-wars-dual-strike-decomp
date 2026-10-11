#include "types.h"
#include "sdk/msl.h"

int vsnprintf(char *buf, u32 size, const char *format, va_list args)
{
    StringSink sink;
    int written;

    sink.buffer = buf;
    sink.capacity = size;
    sink.length = 0;
    written = __pformatter(__StringWrite, &sink, format, args);
    if (buf == NULL) {
        return written;
    }
    if (written < size) {
        buf[written] = '\0';
    } else if (size != 0) {
        (buf + size)[-1] = '\0';
    }
    return written;
}
