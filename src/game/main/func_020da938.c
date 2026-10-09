#include "types.h"

typedef struct Unk_020da938 Unk_020da938;

struct Unk_020da938 {
    u8 pad_00[0x2];
    u16 unk_2;
};

void func_020da938(Unk_020da938 *a0)
{
    a0->unk_2 = a0->unk_2 & -0xc001;
}
