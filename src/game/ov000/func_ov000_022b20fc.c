#include "types.h"

typedef struct Unk_022b20fc_0 Unk_022b20fc_0;

struct Unk_022b20fc_0 {
    u8 pad_00[0x4];
    int unk_4;
};

extern Unk_022b20fc_0 *data_ov000_0234ec8c;

void func_ov000_022b20fc(void)
{
    data_ov000_0234ec8c->unk_4 = 0;
}
