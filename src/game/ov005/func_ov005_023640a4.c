#include "types.h"

typedef struct Unk_023640a4_0 Unk_023640a4_0;

struct Unk_023640a4_0 {
    u8 pad_00[0x68];
    int unk_68;
};

extern Unk_023640a4_0 *data_ov005_0236bbac;

int func_ov005_023640a4(void)
{
    return data_ov005_0236bbac->unk_68;
}
