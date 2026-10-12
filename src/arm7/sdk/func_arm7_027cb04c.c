#include "types.h"

typedef struct Unk_027cb04c_0 Unk_027cb04c_0;

struct Unk_027cb04c_0 {
    u8 pad_00[0x16];
    u16 unk_16;
};

Unk_027cb04c_0 *WifiStation_Entry(void);

u16 func_arm7_027cb04c(void)
{
    Unk_027cb04c_0 *t0 = WifiStation_Entry();
    return t0->unk_16;
}
