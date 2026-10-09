#include "types.h"

int Font_LoadDigits(int);
int PopupMenu_PopState(int);
int MenuDepth_Push(int);

int func_020bc548(void)
{
    int t0 = Font_LoadDigits(0);
    int t1 = PopupMenu_PopState(t0);
    return MenuDepth_Push(t1);
}
