#include "types.h"

u32 func_020280d4(u32 index)
{
    switch (index) {
    case 0:
        return 8;
    case 1:
        return 16;
    case 2:
        return 32;
    case 3:
        return 64;
    default:
        return 0;
    }
}
