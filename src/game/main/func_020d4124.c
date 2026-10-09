#include "types.h"

int FrameCallback_Remove(void *);
void func_020d3ff4(void);
int func_020d41a4(int);

int func_020d4124(void)
{
    int t0 = FrameCallback_Remove((void *)func_020d3ff4);
    return func_020d41a4(t0);
}
