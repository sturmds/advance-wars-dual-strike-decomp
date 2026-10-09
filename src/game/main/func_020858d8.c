#include "types.h"

int Sound_PlaySe(int);
int MenuButtons_CloseAll(int);
int func_020b9864(int);

int func_020858d8(void)
{
    int t0 = Sound_PlaySe(0x10009);
    int t1 = MenuButtons_CloseAll(t0);
    return func_020b9864(t1);
}
