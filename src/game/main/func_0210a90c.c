#include "types.h"

void func_0210a9a8(void *, int, int);
extern u8 data_022a988e[];

void func_0210a90c(void)
{
    int t0 = *((int *)0x4001000) & -0x40000001;
    *((int *)0x4001000) = t0;
    func_0210a9a8(data_022a988e, t0, 0x4001000);
}
