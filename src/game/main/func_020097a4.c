#include "types.h"

int Screen_ResetBgRegisters(void);
int func_020099c8(int);
int func_020097c8(int);
int func_02003ff8(int);

int func_020097a4(void)
{
    int t0 = Screen_ResetBgRegisters();
    int t1 = func_020099c8(t0);
    int t2 = func_020097c8(t1);
    return func_02003ff8(t2);
}
