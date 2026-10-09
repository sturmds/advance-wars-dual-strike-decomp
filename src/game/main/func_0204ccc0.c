#include "types.h"

int Sound_PlaySe(int);
int func_0204cd00(int);
int MenuButtons_CloseAll(int);

int func_0204ccc0(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = func_0204cd00(t0);
    return MenuButtons_CloseAll(t1);
}
