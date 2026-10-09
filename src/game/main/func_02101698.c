#include "types.h"

typedef struct Unk_02101698_0 Unk_02101698_0;

struct Unk_02101698_0 {
    u8 pad_00[0x118];
    int unk_118;
};

extern Unk_02101698_0 *data_022a7ea8;

int func_02101698(void)
{
    return data_022a7ea8->unk_118;
}
