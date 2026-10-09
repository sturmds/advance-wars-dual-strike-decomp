#include "types.h"

int Screen_ResetBgRegisters(void);
int Screen_ResetBlend(int);
int Screen_ResetWindows(int);
int func_02003ff8(int);

int func_020097a4(void)
{
    int t0 = Screen_ResetBgRegisters();
    int t1 = Screen_ResetBlend(t0);
    int t2 = Screen_ResetWindows(t1);
    return func_02003ff8(t2);
}
