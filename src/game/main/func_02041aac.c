#include "types.h"

int Sound_PlaySe(int);
int CoSelectScreen_BeginExit(int);
int func_020b9890(int);

int func_02041aac(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = CoSelectScreen_BeginExit(t0);
    return func_020b9890(t1);
}
