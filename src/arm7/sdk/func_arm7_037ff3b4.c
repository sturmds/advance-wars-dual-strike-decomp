#include "types.h"

typedef struct Unk_020f137c Unk_020f137c;
struct Unk_020f137c {
    u8 pad_00[0x6];
    u16 unk_44;
    u16 unk_46;
};

void func_arm7_037ff3b4(Unk_020f137c *a0)
{
    a0->unk_46 = 0;
    a0->unk_44 = 0;
}
