#include "types.h"

extern u32 gWirelessState;
extern void Wireless_OnStepDone(void);

BOOL func_02079578(void)
{
    if (func_02117d3c(Wireless_OnStepDone) != 2) {
        gWirelessState = 1;
        return FALSE;
    }
    return TRUE;
}
