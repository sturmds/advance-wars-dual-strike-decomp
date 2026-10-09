#include "types.h"

typedef struct Unk_020c3e34_0 Unk_020c3e34_0;
typedef struct Unk_020c3e34_1 Unk_020c3e34_1;

struct Unk_020c3e34_0 {
    u8 pad_00[0x48];
    Unk_020c3e34_1 *unk_48[1];
};

struct Unk_020c3e34_1 {
    u8 pad_00[0x1c4];
    int unk_1c4;
};

extern Unk_020c3e34_0 *data_027c027c;
extern int data_027c0284;

int func_020c3e34(void)
{
    Unk_020c3e34_0 *t0 = data_027c027c;
    int t1 = data_027c0284;
    return t0->unk_48[t1]->unk_1c4;
}
