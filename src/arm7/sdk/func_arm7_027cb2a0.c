#include "types.h"

typedef struct Unk_027cb2a0_0 Unk_027cb2a0_0;

struct Unk_027cb2a0_0 {
    u8 pad_00[0xe];
    u16 unk_e;
};

Unk_027cb2a0_0 *WifiStation_Entry(void);

void func_arm7_027cb2a0(int a0, int a1)
{
    Unk_027cb2a0_0 *t0 = WifiStation_Entry();
    t0->unk_e = a1;
}
