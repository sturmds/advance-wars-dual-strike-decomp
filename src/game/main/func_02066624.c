#include "types.h"

int Sound_PlaySe(int);
int func_020b9890(int);
int VsSetup_OpenCoSelect(int);

int func_02066624(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = func_020b9890(t0);
    return VsSetup_OpenCoSelect(t1);
}
