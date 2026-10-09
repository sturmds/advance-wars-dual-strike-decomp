#include "types.h"

typedef struct Unk_023630c0_0 Unk_023630c0_0;

struct Unk_023630c0_0 {
    u8 pad_00[0x20c];
    int unk_20c;
    u8 pad_210[0x4];
    int unk_214;
};

extern Unk_023630c0_0 *data_ov005_0236bba8;

void func_ov005_023630c0(u8 *a0)
{
    Unk_023630c0_0 *t0 = data_ov005_0236bba8;
    int t1 = *a0 - 224;
    t0->unk_20c = t1;
    if (t1 != 8) {
        t0->unk_214 = t1;
    }
}
