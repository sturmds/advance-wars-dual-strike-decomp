#include "types.h"

int Task_Create(void *, int);
extern u8 data_0215fea0[];
int func_0207a3e0(int);

int func_0207bbd0(void)
{
    int t0 = Task_Create(data_0215fea0, 0);
    return func_0207a3e0(t0);
}
