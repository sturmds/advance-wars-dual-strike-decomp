#include "types.h"

typedef struct Unk_0210e494_0 Unk_0210e494_0;

struct Unk_0210e494_0 {
    u8 pad_00[0x27ffda0];
    int unk_27ffda0;
};

int OS_GetArenaLo(int a0)
{
    return ((Unk_0210e494_0 *)(a0 << 2))->unk_27ffda0;
}
