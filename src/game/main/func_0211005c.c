#include "types.h"

int func_021100bc(void);
int func_0211007c(int);

int func_0211005c(void)
{
    int t0 = func_021100bc();
    int t1 = func_0211007c(t0);
    return (0x100 - t0) - t1;
}
