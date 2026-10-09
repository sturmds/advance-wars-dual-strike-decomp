#include "types.h"

typedef struct Unk_0228ff58 Unk_0228ff58;

struct Unk_0228ff58 {
    u8 pad_00[0x18];
    u16 unk_18;
};

extern Unk_0228ff58 data_0228ff58;

int func_0200c020(void)
{
    return data_0228ff58.unk_18 & 255;
}
