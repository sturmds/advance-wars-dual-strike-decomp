#include "types.h"

typedef struct Unk_038037fc_0 Unk_038037fc_0;

struct Unk_038037fc_0 {
    u8 pad_00[0xc];
    int unk_c;
    int unk_10;
    int unk_14;
};

void func_arm7_03802c8c(int, int, int);

void func_arm7_038037fc(Unk_038037fc_0 **a0)
{
    Unk_038037fc_0 *t0 = *a0;
    func_arm7_03802c8c(t0->unk_10, t0->unk_c, t0->unk_14);
}
