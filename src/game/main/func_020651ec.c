#include "types.h"

int Sound_PlaySe(int);
int func_020b9864(int);
int func_020654dc(int);

int func_020651ec(void)
{
    int t0 = Sound_PlaySe(0x10009);
    int t1 = func_020b9864(t0);
    return func_020654dc(t1);
}
