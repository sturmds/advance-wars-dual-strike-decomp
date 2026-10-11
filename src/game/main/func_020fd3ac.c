#include "types.h"

typedef struct Unk_020fd3ac Unk_020fd3ac;

struct Unk_020fd3ac {
    u8 pad_00[0x4c];
    int unk_4c;
    int unk_50;
};

void PaletteFade_Update(void *, int, int);

void func_020fd3ac(Unk_020fd3ac *a0)
{
    PaletteFade_Update(a0, a0->unk_4c, a0->unk_50);
}
