#include "types.h"

extern u32 data_0229c88c;
extern void func_020793a4(void);

BOOL func_02079368(void)
{
    if (func_02117cac(func_020793a4) != 2) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
