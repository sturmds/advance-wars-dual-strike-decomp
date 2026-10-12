#include "types.h"

typedef struct Unk_03803850_0 Unk_03803850_0;

struct Unk_03803850_0 {
    u8 pad_00[0xc];
    int unk_c;
    int unk_10;
    int unk_14;
};

void func_arm7_03802ecc(int, int, int);

void func_arm7_03803850(Unk_03803850_0 **a0)
{
    Unk_03803850_0 *t0 = *a0;
    func_arm7_03802ecc(t0->unk_c, t0->unk_10, t0->unk_14);
}
