#include "types.h"

int Sound_PlaySe(int);
int CoSelectScreen_BeginExit(int);
int func_020b9864(int);

int func_020428f4(void)
{
    int t0 = Sound_PlaySe(0x10009);
    int t1 = CoSelectScreen_BeginExit(t0);
    return func_020b9864(t1);
}
