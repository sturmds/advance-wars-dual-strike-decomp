#include "types.h"

typedef struct Unk_037ff7d8 Unk_037ff7d8;

struct Unk_037ff7d8 {
    u8 pad_00[0x3];
    u8 unk_3;
};

int func_arm7_037ff7d8(Unk_037ff7d8 *a0)
{
    return (u32)(a0->unk_3 << 31) >> 31;
}
