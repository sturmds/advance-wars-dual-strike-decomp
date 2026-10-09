#include "types.h"

int Sound_PlaySe(int);
int Script_StartWithHook(int);

int func_020d1420(void)
{
    int t0 = Sound_PlaySe(69);
    return Script_StartWithHook(t0);
}
