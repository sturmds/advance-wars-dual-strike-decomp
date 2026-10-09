#include "types.h"

typedef struct Unk_02363e40_0 Unk_02363e40_0;

struct Unk_02363e40_0 {
    u8 pad_00[0x1da];
    u8 unk_1da;
};

extern Unk_02363e40_0 *data_ov003_02364928;

int func_ov003_02363e40(void)
{
    Unk_02363e40_0 *t0 = data_ov003_02364928;
    u8 t1 = t0->unk_1da;
    int t2 = t1 & -241;
    int t3 = t2 | 16;
    t0->unk_1da = t3;
    return t3;
}
