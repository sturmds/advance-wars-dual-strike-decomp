#include "types.h"

typedef struct Unk_02367d4c_0 Unk_02367d4c_0;

struct Unk_02367d4c_0 {
    u8 pad_00[0x4];
    int unk_4;
    u8 pad_08[0x1c];
    int unk_24;
};

extern Unk_02367d4c_0 *data_ov005_0236bbb0;

int func_ov005_02367d4c(u8 *a0)
{
    u8 t0 = *a0;
    Unk_02367d4c_0 *t1 = data_ov005_0236bbb0;
    int t2 = t0 - 128;
    t1->unk_24 = t2;
    t1->unk_4 = 1;
    return t2;
}
