#include "types.h"

int Sound_PlaySe(int);
int MenuButtons_CloseAll(int);
int func_020b9890(int);

int func_020877b8(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = MenuButtons_CloseAll(t0);
    return func_020b9890(t1);
}
