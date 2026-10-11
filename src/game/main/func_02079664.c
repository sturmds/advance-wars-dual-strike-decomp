#include "types.h"

extern u32 gWirelessState;
extern void func_020796a0(void);

BOOL func_02079664(void)
{
    if (func_02117d84(func_020796a0) != 2) {
        gWirelessState = 1;
        return FALSE;
    }
    return TRUE;
}
