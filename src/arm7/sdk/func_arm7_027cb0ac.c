#include "types.h"

typedef struct Unk_027cb0ac_0 Unk_027cb0ac_0;

struct Unk_027cb0ac_0 {
    u8 pad_00[0x14];
    u16 unk_14;
};

Unk_027cb0ac_0 *WifiStation_Entry(void);

u16 func_arm7_027cb0ac(void)
{
    Unk_027cb0ac_0 *t0 = WifiStation_Entry();
    return t0->unk_14;
}
