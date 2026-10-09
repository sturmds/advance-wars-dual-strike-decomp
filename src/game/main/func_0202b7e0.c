#include "types.h"

void func_020030cc(void *, int, int, int);
extern u8 data_02159928[];

void func_0202b7e0(int a0)
{
    func_020030cc(data_02159928, 0, (u32)(a0 << 21) >> 16, 32);
}
