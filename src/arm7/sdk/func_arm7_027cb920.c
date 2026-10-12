#include "types.h"

typedef struct Unk_027cb920_0 Unk_027cb920_0;

struct Unk_027cb920_0 {
    u8 pad_00[0x3fc];
    u16 unk_3fc;
};

void func_arm7_027cb920(void)
{
    Unk_027cb920_0 *t0 = *((Unk_027cb920_0 **)0x380fff4);
    t0->unk_3fc = 0;
}
