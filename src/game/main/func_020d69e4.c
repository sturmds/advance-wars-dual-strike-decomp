#include "types.h"

int func_020097c8(void);
void Screen_ResetBlend(int);
int func_02031480(int);

int func_020d69e4(int a0, int a1)
{
    int t0 = func_020097c8();
    Screen_ResetBlend(t0);
    return func_02031480(a1);
}
