#include "types.h"

int func_021100bc(void);
int Sound_PendingCommandCount(int);

int func_0211005c(void)
{
    int t0 = func_021100bc();
    int t1 = Sound_PendingCommandCount(t0);
    return (0x100 - t0) - t1;
}
