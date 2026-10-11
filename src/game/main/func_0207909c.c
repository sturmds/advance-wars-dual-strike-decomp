#include "types.h"

void Wireless_OnParentParamSet(void *event);
int func_02117ab4(void (*callback)(void *event), int arg);
extern int data_0229c8a0;

BOOL func_0207909c(void)
{
    return func_02117ab4(Wireless_OnParentParamSet, data_0229c8a0) == 2;
}
