#include "types.h"

typedef struct Unk_0235cd34_0 Unk_0235cd34_0;
typedef struct Unk_0235cd34 Unk_0235cd34;

struct Unk_0235cd34_0 {
    u8 pad_00[0x4];
    int unk_4;
    int unk_8;
    int unk_c;
    int unk_10;
};

struct Unk_0235cd34 {
    u8 pad_00[0x2c];
    Unk_0235cd34_0 *unk_2c;
};

int func_ov005_0235cd34(Unk_0235cd34 *a0)
{
    Unk_0235cd34_0 *t0 = a0->unk_2c;
    t0->unk_4 = 0;
    t0->unk_8 = 0;
    t0->unk_c = 0;
    t0->unk_10 = 0;
    return 0;
}
