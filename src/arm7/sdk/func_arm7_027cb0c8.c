#include "types.h"

typedef struct Unk_027cb0c8_0 Unk_027cb0c8_0;

struct Unk_027cb0c8_0 {
    u8 pad_00[0xe];
    u16 unk_e;
};

Unk_027cb0c8_0 *WifiStation_Entry(void);

u16 func_arm7_027cb0c8(void)
{
    Unk_027cb0c8_0 *t0 = WifiStation_Entry();
    return t0->unk_e;
}
