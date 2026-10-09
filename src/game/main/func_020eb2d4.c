#include "types.h"

typedef struct Unk_022a7b80 Unk_022a7b80;

struct Unk_022a7b80 {
    u8 pad_00[0x6];
    u8 unk_6;
};

extern Unk_022a7b80 data_022a7b80;

void * func_020eb2d4(void)
{
    data_022a7b80.unk_6 = 0;
    return &data_022a7b80;
}
