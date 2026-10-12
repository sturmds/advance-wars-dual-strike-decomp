#include "types.h"

typedef struct Unk_037ff7f4 Unk_037ff7f4;

struct Unk_037ff7f4 {
    u8 pad_00[0x20];
    u16 unk_20;
};

int SndExChannel_CalcDecayRate(int);

int func_arm7_037ff7f4(Unk_037ff7f4 *a0, int a1)
{
    int t0 = SndExChannel_CalcDecayRate(a1);
    a0->unk_20 = t0;
    return t0;
}
