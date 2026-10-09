#include "types.h"

int Sound_PlaySe(int);
int func_020b9864(int);
int func_02046f74(int);

int func_02046e3c(void)
{
    int t0 = Sound_PlaySe(0x10009);
    int t1 = func_020b9864(t0);
    return func_02046f74(t1);
}
