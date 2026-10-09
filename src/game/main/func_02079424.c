#include "types.h"

extern u32 data_0229c88c;
extern void func_02079460(void);

BOOL func_02079424(void)
{
    if (func_02117cf4(func_02079460) != 2) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
