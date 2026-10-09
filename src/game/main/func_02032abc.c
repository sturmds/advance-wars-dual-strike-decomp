#include "types.h"

int Sound_PlaySe(int);
int MenuButtons_CloseAll(int);
int func_0203579c(int);

int func_02032abc(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = MenuButtons_CloseAll(t0);
    return func_0203579c(t1);
}
