#include "types.h"

extern u32 data_022a7e68;
extern void func_020fc670(void);

BOOL func_020fc634(void)
{
    if (func_02117d3c(func_020fc670) != 2) {
        data_022a7e68 = 6;
        return FALSE;
    }
    return TRUE;
}
