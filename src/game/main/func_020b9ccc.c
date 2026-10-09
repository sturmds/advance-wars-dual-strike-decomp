#include "types.h"

typedef struct Unk_020b9ccc_0 Unk_020b9ccc_0;
typedef struct Unk_020b9ccc_1 Unk_020b9ccc_1;

struct Unk_020b9ccc_0 {
    u8 pad_00[0x48];
    Unk_020b9ccc_1 *unk_48[1];
};

struct Unk_020b9ccc_1 {
    u8 pad_00[0x8];
    s8 unk_8;
};

extern Unk_020b9ccc_0 *data_027c027c;
extern int data_027c0284;

s8 func_020b9ccc(void)
{
    Unk_020b9ccc_0 *t0 = data_027c027c;
    int t1 = data_027c0284;
    return t0->unk_48[t1]->unk_8;
}
