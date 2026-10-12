#include "types.h"

typedef struct Unk_038037e8_0 Unk_038037e8_0;

struct Unk_038037e8_0 {
    u8 pad_00[0x10];
    int unk_10;
};

void CardSave_EraseSectorAt(int);

void func_arm7_038037e8(Unk_038037e8_0 **a0)
{
    Unk_038037e8_0 *t0 = *a0;
    int t1 = t0->unk_10;
    CardSave_EraseSectorAt(t1);
}
