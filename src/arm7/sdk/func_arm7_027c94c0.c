#include "types.h"

typedef struct Unk_027c94c0_0 Unk_027c94c0_0;

struct Unk_027c94c0_0 {
    u8 pad_00[0x328];
    u16 unk_328;
};

int func_arm7_027c94c0(int a0)
{
    Unk_027c94c0_0 *t0 = *((Unk_027c94c0_0 **)0x380fff4);
    t0->unk_328 = (u32)a0 >> 1;
    *((u16 *)0x480803c) = a0;
    return 0;
}
