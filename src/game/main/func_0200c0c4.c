#include "types.h"

typedef struct Unk_0200c0c4_0 Unk_0200c0c4_0;

struct Unk_0200c0c4_0 {
    u8 pad_00[0x6a];
    u16 unk_6a;
};

extern Unk_0200c0c4_0 *data_027c0280;
void OS_DisableIrqMask(int, int, void *);

void func_0200c0c4(void)
{
    Unk_0200c0c4_0 *t0 = data_027c0280;
    int t1 = t0->unk_6a & -17;
    t0->unk_6a = t1;
    OS_DisableIrqMask(2, t1, t0);
}
