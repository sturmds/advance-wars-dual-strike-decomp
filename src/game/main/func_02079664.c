#include "types.h"

extern u32 data_0229c88c;
extern void func_020796a0(void);

BOOL func_02079664(void)
{
    if (func_02117d84(func_020796a0) != 2) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
