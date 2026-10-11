#include "types.h"

extern u16 gGxVramBankMasks;
void Vram_PowerOnBanks(int, void *, int);

void func_0210ac20(int a0)
{
    int t0 = gGxVramBankMasks | a0;
    gGxVramBankMasks = t0;
    Vram_PowerOnBanks(a0, &gGxVramBankMasks, t0);
}
