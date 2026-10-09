#include "types.h"

typedef struct Unk_02367bd0 Unk_02367bd0;

struct Unk_02367bd0 {
    u8 pad_00[0x34];
    int unk_34;
    u8 pad_38[0xd0];
    int unk_108;
};

int func_ov005_02367bd0(Unk_02367bd0 *a0)
{
    return a0->unk_108 - (a0->unk_34 * 6);
}
