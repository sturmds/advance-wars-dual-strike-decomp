#include "types.h"

int strlen(void);

int func_020babb4(void)
{
    int t0 = strlen();
    return (0x100 - (t0 << 3)) >> 1;
}
