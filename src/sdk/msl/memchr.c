#include "types.h"
#include "sdk/msl.h"

void *memchr(const void *buf, int ch, u32 count)
{
    const u8 *p = buf;
    u8 target = ch;

    while (count != 0) {
        if (*p++ == target) {
            return (void *)(p - 1);
        }
        count--;
    }
    return NULL;
}
