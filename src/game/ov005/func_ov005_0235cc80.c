#include "types.h"

typedef struct PlayRecords PlayRecords;

struct PlayRecords {
    u8 pad_00[0x14];
    u8 unk_14;
};

extern PlayRecords gPlayRecords;
extern u8 data_ov005_0236a48c[];

u8 func_ov005_0235cc80(int a0)
{
    gPlayRecords.unk_14 = data_ov005_0236a48c[a0];
    return data_ov005_0236a48c[a0];
}
