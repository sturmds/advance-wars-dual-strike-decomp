#include "types.h"

typedef struct Unk_027cb068_0 Unk_027cb068_0;

struct Unk_027cb068_0 {
    u8 pad_00[0x2];
    u16 unk_2;
};

Unk_027cb068_0 *WifiStation_Entry(void);

u16 func_arm7_027cb068(void)
{
    Unk_027cb068_0 *t0 = WifiStation_Entry();
    return t0->unk_2;
}
