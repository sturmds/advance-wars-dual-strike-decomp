#include "types.h"

typedef struct Unk_0380386c_0 Unk_0380386c_0;

struct Unk_0380386c_0 {
    u8 pad_00[0xc];
    int unk_c;
    int unk_10;
    int unk_14;
};

void CardRom_BeginCpuRead(int, int, int, int);

void func_arm7_0380386c(Unk_0380386c_0 **a0)
{
    Unk_0380386c_0 *t0 = *a0;
    CardRom_BeginCpuRead(4, t0->unk_c, t0->unk_10, t0->unk_14);
}
