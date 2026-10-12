#include "types.h"

typedef struct Unk_027cb120_0 Unk_027cb120_0;

struct Unk_027cb120_0 {
    u8 pad_00[0x508];
    u16 unk_508;
};

int func_arm7_027cb120(int a0)
{
    Unk_027cb120_0 *t0 = *((Unk_027cb120_0 **)0x380fff4);
    return (t0->unk_508 >> a0) & 1;
}
