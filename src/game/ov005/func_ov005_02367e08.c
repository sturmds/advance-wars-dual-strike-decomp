#include "types.h"

typedef struct Unk_02367e08_0 Unk_02367e08_0;

struct Unk_02367e08_0 {
    u8 pad_00[0x10];
    int unk_10;
};

extern Unk_02367e08_0 *data_ov005_0236bbb0;

void func_ov005_02367e08(void)
{
    data_ov005_0236bbb0->unk_10 = 1;
}
