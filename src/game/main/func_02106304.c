#include "types.h"

typedef struct Unk_02106304_0 Unk_02106304_0;
typedef struct Unk_02106304_1 Unk_02106304_1;
typedef struct Unk_02106304_2 Unk_02106304_2;

struct Unk_02106304_0 {
    u8 pad_00[0x8];
    int unk_8;
};

struct Unk_02106304_1 {
    u8 pad_00[0x14];
    int unk_14;
};

struct Unk_02106304_2 {
    u8 pad_00[0x80];
    Unk_02106304_0 *unk_80;
};

extern Unk_02106304_2 *data_022a8a60;

int func_02106304(int a0)
{
    Unk_02106304_0 *t0 = data_022a8a60->unk_80;
    if ((u32)a0 < (u32)t0->unk_8) {
    }
    return ((u32)a0 >= (u32)t0->unk_8) ? 0 : ((Unk_02106304_1 *)((int)t0 + (a0 << 4)))->unk_14;
}
