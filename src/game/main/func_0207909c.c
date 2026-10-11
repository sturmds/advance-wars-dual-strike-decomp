#include "types.h"

void Wireless_OnParentParamSet(void *event);
int WM_SetParentParameter(void (*callback)(void *event), int arg);
extern int data_0229c8a0;

BOOL func_0207909c(void)
{
    return WM_SetParentParameter(Wireless_OnParentParamSet, data_0229c8a0) == 2;
}
