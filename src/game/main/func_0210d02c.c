#include "types.h"

int func_0210d02c(void)
{
    u16 t0 = *((u16 *)0x4000204);
    int t1 = t0 & -129;
    *((u16 *)0x4000204) = t1;
    return t1;
}
