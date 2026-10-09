#include "types.h"

int func_020097c8(void);
void Screen_ResetBlend(int);
int func_020f4c00(int, int);

int func_020d69c0(int a0)
{
    int t0 = func_020097c8();
    Screen_ResetBlend(t0);
    return func_020f4c00(1, a0);
}
