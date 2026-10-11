#include "types.h"

/* Larger of two unsigned values. */
u32 MbMaxU32(u32 a, u32 b)
{
    if (b <= a) {
        b = a;
    }
    return b;
}
