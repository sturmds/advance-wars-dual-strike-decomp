#include "types.h"

typedef struct Unk_023640b8_0 Unk_023640b8_0;

struct Unk_023640b8_0 {
    u8 pad_00[0x58];
    int unk_58;
    u8 pad_5c[0x4];
    int unk_60;
};

extern Unk_023640b8_0 *data_ov005_0236bbac;

void func_ov005_023640b8(void)
{
    Unk_023640b8_0 *t0 = data_ov005_0236bbac;
    t0->unk_58 = -1;
    t0->unk_60 = -1;
}
