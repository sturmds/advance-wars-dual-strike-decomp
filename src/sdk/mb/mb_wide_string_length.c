#include "types.h"

int MbWideString_Length(const u16 *string)
{
    int length = 0;

    while (*string++ != 0) {
        length++;
    }
    return length;
}
