#include "types.h"

extern u32 gWirelessState;
extern void func_02079460(void);

BOOL func_02079424(void)
{
    if (WM_PowerOn(func_02079460) != 2) {
        gWirelessState = 1;
        return FALSE;
    }
    return TRUE;
}
