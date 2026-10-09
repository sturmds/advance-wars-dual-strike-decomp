#include "types.h"

typedef struct Unk_023531f4 Unk_023531f4;
typedef struct Unk_023531f4_1 Unk_023531f4_1;

struct Unk_023531f4 {
    u8 pad_00[0x2c];
    Unk_023531f4_1 *unk_2c;
};

struct Unk_023531f4_1 {
    u8 pad_00[0x78];
    int unk_78;
};

void func_ov005_023531f4(Unk_023531f4 *a0)
{
    a0->unk_2c->unk_78 = 1;
}
