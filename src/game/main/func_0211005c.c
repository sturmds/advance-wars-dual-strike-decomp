#include "types.h"

int Sound_FreeCommandCount(void);
int Sound_PendingCommandCount(int);

int func_0211005c(void)
{
    int t0 = Sound_FreeCommandCount();
    int t1 = Sound_PendingCommandCount(t0);
    return (0x100 - t0) - t1;
}
