#include "types.h"

typedef struct Unk_038010e4 Unk_038010e4;

struct Unk_038010e4 {
    u8 pad_00[0x24];
    int unk_24;
    int unk_28;
};

void func_arm7_038010e4(Unk_038010e4 *a0, int a1, int a2)
{
    a0->unk_24 = a1;
    a0->unk_28 = a0->unk_24 + a2;
}
