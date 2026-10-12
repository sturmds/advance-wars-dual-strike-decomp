#include "types.h"

typedef struct Unk_027cb2d0_0 Unk_027cb2d0_0;

struct Unk_027cb2d0_0 {
    u8 pad_00[0x10];
    u16 unk_10;
};

Unk_027cb2d0_0 *func_arm7_027cb030(void);

void func_arm7_027cb2d0(int a0, int a1)
{
    Unk_027cb2d0_0 *t0 = func_arm7_027cb030();
    t0->unk_10 = a1;
}
