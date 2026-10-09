#include "types.h"

int Font_LoadDigits(int);
int PopupMenu_PopState(int);
int func_020b9c9c(int);

int func_020bc548(void)
{
    int t0 = Font_LoadDigits(0);
    int t1 = PopupMenu_PopState(t0);
    return func_020b9c9c(t1);
}
