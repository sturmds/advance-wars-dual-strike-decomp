#include "types.h"

int Sound_PlaySe(int);
int func_020b9890(int);
int func_02069b14(int);

int func_02067b0c(void)
{
    int t0 = Sound_PlaySe(0x10006);
    int t1 = func_020b9890(t0);
    return func_02069b14(t1);
}
