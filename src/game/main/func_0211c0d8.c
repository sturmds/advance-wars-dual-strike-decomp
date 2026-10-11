#include "types.h"

typedef struct Unk_0211c0d8_0 Unk_0211c0d8_0;

struct Unk_0211c0d8_0 {
    u8 pad_00[0x1320];
    int unk_1320;
};

extern Unk_0211c0d8_0 *data_022aced4;
void MbParent_StartCore(int, int, int);

void func_0211c0d8(int a0)
{
    Unk_0211c0d8_0 *t0 = data_022aced4;
    t0->unk_1320 = 1;
    MbParent_StartCore(a0, (int)t0 + 0x1000, 1);
}
