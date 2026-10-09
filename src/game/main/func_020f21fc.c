#include "types.h"

typedef struct Unk_020f21fc_0 Unk_020f21fc_0;

struct Unk_020f21fc_0 {
    u8 pad_00[0x14];
    int unk_14;
};

extern Unk_020f21fc_0 *data_021694c0;

int func_020f21fc(void)
{
    return data_021694c0->unk_14;
}
