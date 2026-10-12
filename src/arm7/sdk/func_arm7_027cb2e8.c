#include "types.h"

typedef struct Unk_027cb2e8_0 Unk_027cb2e8_0;

struct Unk_027cb2e8_0 {
    u8 pad_00[0xc];
    u16 unk_c;
};

Unk_027cb2e8_0 *func_arm7_027cb030(void);

void func_arm7_027cb2e8(int a0, int a1)
{
    Unk_027cb2e8_0 *t0 = func_arm7_027cb030();
    t0->unk_c = a1;
}
