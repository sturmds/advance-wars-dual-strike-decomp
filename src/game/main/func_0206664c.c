#include "types.h"

int Sound_PlaySe(int);
int func_020b9864(int);
int VsSetup_OpenCoSelect(int);

int func_0206664c(void)
{
    int t0 = Sound_PlaySe(0x10009);
    int t1 = func_020b9864(t0);
    return VsSetup_OpenCoSelect(t1);
}
