#include "types.h"

typedef struct Unk_0228ff58 Unk_0228ff58;

struct Unk_0228ff58 {
    u8 pad_00[0x1];
    u8 unk_1;
};

extern Unk_0228ff58 data_0228ff58;

u8 func_0200c034(void)
{
    return data_0228ff58.unk_1;
}
