#include "types.h"

typedef struct Unk_037fe73c_0 Unk_037fe73c_0;

struct Unk_037fe73c_0 {
    u8 pad_00[0x4000400];
    int unk_4000400;
};

int func_arm7_037fe73c(int a0)
{
    return ((Unk_037fe73c_0 *)(a0 << 4))->unk_4000400;
}
