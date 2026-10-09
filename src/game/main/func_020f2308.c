#include "types.h"

typedef struct Unk_020f2308_0 Unk_020f2308_0;

struct Unk_020f2308_0 {
    u8 pad_00[0x38];
    int unk_38;
    int unk_3c;
};

extern Unk_020f2308_0 *data_021694c0;

int func_020f2308(void)
{
    Unk_020f2308_0 *t0 = data_021694c0;
    return (t0->unk_3c == t0->unk_38) ? 1 : 0;
}
