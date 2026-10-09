#include "types.h"

typedef struct Unk_020f4638_0 Unk_020f4638_0;

struct Unk_020f4638_0 {
    u8 pad_00[0x8];
    int unk_8;
};

extern Unk_020f4638_0 *data_021694c0;

int func_020f4638(int a0)
{
    return (a0 == data_021694c0->unk_8) ? 1 : 0;
}
