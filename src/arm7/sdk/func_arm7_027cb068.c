#include "types.h"

typedef struct Unk_027cb068_0 Unk_027cb068_0;

struct Unk_027cb068_0 {
    u8 pad_00[0x2];
    u16 unk_2;
};

Unk_027cb068_0 *func_arm7_027cb030(void);

u16 func_arm7_027cb068(void)
{
    Unk_027cb068_0 *t0 = func_arm7_027cb030();
    return t0->unk_2;
}
