#include "types.h"

int Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(int);
int func_020077d4(int);

int func_02006610(void)
{
    int t0 = Screen_EnableEmptyWindows();
    Screen_ResetWindowRects(t0);
    return func_020077d4(1);
}
