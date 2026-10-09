#include "types.h"

typedef struct Unk_02044c04_0 Unk_02044c04_0;

struct Unk_02044c04_0 {
    u8 pad_00[0x36];
    u8 unk_36;
};

extern Unk_02044c04_0 *data_0229c73c;

int func_02044c04(int a0)
{
    return (data_0229c73c->unk_36 == 2) ? (a0 << 1) : ((a0 << 1) + 1);
}
