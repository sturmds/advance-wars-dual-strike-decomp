#include "types.h"

extern u16 *data_0215fe84;
void Wireless_CountChildren(int, void *);

void func_0207a1c0(int a0)
{
    u16 *t0 = data_0215fe84;
    *t0 = a0;
    Wireless_CountChildren(a0, t0);
}
