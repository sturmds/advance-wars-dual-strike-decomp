#include "types.h"

int Palette_GetBlock(int, int);
int Mem_Copy32(int, int, int);

int func_0203c6d8(int a0, int a1)
{
    int t0 = Palette_GetBlock(a0, 1);
    return Mem_Copy32(t0, a1, 0x100);
}
