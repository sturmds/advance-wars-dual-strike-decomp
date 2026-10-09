#include "types.h"

typedef struct Unk_020f21e8_0 Unk_020f21e8_0;

struct Unk_020f21e8_0 {
    u8 pad_00[0x18];
    int unk_18;
};

extern Unk_020f21e8_0 *data_021694c0;

void func_020f21e8(int a0)
{
    data_021694c0->unk_18 = a0;
}
