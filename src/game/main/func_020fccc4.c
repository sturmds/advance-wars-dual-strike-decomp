#include "types.h"

typedef struct Unk_020fccc4 Unk_020fccc4;

struct Unk_020fccc4 {
    u8 pad_00[0x30];
    int unk_30;
    int unk_34;
};

void func_020fccc4(Unk_020fccc4 *a0)
{
    a0->unk_34 = 0;
    a0->unk_30 = 1;
}
