#include "types.h"

extern u16 data_022a9878;
void Vram_PowerOnBanks(int, void *, int);

void func_0210ac20(int a0)
{
    int t0 = data_022a9878 | a0;
    data_022a9878 = t0;
    Vram_PowerOnBanks(a0, &data_022a9878, t0);
}
