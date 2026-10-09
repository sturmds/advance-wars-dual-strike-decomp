#include "types.h"

extern u16 *data_0215fe84;
void func_0207a208(int, void *);

void func_0207a1c0(int a0)
{
    u16 *t0 = data_0215fe84;
    *t0 = a0;
    func_0207a208(a0, t0);
}
