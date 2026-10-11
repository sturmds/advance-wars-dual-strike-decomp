#include "types.h"
#include "sdk/msl.h"

u32 strlen(const char *str)
{
    u32 length = -1;

    do {
        length++;
    } while (*str++ != '\0');
    return length;
}
