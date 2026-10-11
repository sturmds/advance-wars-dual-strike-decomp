#include "types.h"

extern u32 data_0229c88c;
extern void Wireless_OnStepDone(void);

BOOL func_02079578(void)
{
    if (func_02117d3c(Wireless_OnStepDone) != 2) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
