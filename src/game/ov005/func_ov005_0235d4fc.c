#include "types.h"

typedef struct Unk_0235d4fc_0 Unk_0235d4fc_0;

struct Unk_0235d4fc_0 {
    u8 pad_00[0x871];
    u8 unk_871;
};

int func_ov005_0235d4fc(int a0, int a1)
{
    return (((Unk_0235d4fc_0 *)(a0 + a1))->unk_871 != 0) ? 1 : 0;
}
