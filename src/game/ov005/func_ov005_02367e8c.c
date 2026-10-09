#include "types.h"

typedef struct PlayRecords PlayRecords;

struct PlayRecords {
    u8 pad_00[0x13];
    u8 unk_13;
};

extern PlayRecords gPlayRecords;

u8 func_ov005_02367e8c(void)
{
    return gPlayRecords.unk_13;
}
