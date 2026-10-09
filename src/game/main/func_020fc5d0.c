#include "types.h"

extern u32 data_022a7e68;
extern void func_020fc60c(void);

BOOL func_020fc5d0(void)
{
    if (func_02117cf4(func_020fc60c) != 2) {
        data_022a7e68 = 5;
        return FALSE;
    }
    return TRUE;
}
