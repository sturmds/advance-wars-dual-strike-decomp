#include "types.h"

typedef struct Unk_03801e94_0 Unk_03801e94_0;

struct Unk_03801e94_0 {
    u8 pad_00[0x260];
    u16 unk_260[1];
};

extern Unk_03801e94_0 *data_arm7_0380808c;

int func_arm7_03801e94(int a0, int a1)
{
    Unk_03801e94_0 *t0 = data_arm7_0380808c;
    t0->unk_260[a0] = a1;
    return ((int)t0 + (a0 << 1)) + 0x200;
}
