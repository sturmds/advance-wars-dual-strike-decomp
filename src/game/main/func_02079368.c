#include "types.h"

extern u32 data_0229c88c;
extern void Wireless_OnEventNextStep(void);

BOOL func_02079368(void)
{
    if (func_02117cac(Wireless_OnEventNextStep) != 2) {
        data_0229c88c = 1;
        return FALSE;
    }
    return TRUE;
}
