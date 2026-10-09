#include "types.h"

typedef struct Unk_021016ac_0 Unk_021016ac_0;

struct Unk_021016ac_0 {
    u8 pad_00[0x118];
    int unk_118;
};

extern Unk_021016ac_0 *data_022a7ea8;

void func_021016ac(void)
{
    data_022a7ea8->unk_118 = 0;
}
