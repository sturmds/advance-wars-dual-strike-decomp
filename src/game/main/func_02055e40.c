#include "types.h"

typedef struct Unk_02055e40 Unk_02055e40;

struct Unk_02055e40 {
    u8 pad_00[0x58];
    int unk_58;
    int unk_5c;
};

void func_02055e40(Unk_02055e40 *a0)
{
    a0->unk_58 = 0;
    a0->unk_5c = 0;
}
