#include "types.h"

typedef struct Unk_027cb3c8_0 Unk_027cb3c8_0;

struct Unk_027cb3c8_0 {
    u8 pad_00[0xa];
    u16 unk_a;
};

Unk_027cb3c8_0 *WifiStation_Entry(void);

void func_arm7_027cb3c8(int a0, int a1)
{
    Unk_027cb3c8_0 *t0 = WifiStation_Entry();
    t0->unk_a = a1;
}
