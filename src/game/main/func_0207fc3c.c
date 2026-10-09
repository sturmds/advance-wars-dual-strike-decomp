#include "types.h"

typedef struct Unk_0207fc3c Unk_0207fc3c;

struct Unk_0207fc3c {
    u8 pad_00[0x13];
    u8 unk_13;
};

int func_0207fc3c(Unk_0207fc3c *a0)
{
    return (a0->unk_13 != 255) ? 1 : 0;
}
