#include "types.h"

typedef struct Unk_02367d28_0 Unk_02367d28_0;

struct Unk_02367d28_0 {
    u8 pad_00[0x24];
    int unk_24;
    u8 pad_28[0x4];
    int unk_2c;
};

extern Unk_02367d28_0 *data_ov005_0236bbb0;

void func_ov005_02367d28(u8 *a0)
{
    Unk_02367d28_0 *t0 = data_ov005_0236bbb0;
    t0->unk_24 = *a0 - 128;
    t0->unk_2c = t0->unk_24;
}
