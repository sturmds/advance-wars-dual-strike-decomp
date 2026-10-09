#include "types.h"

typedef struct PlayRecords PlayRecords;

struct PlayRecords {
    u8 pad_00[0x13];
    u8 unk_13;
};

extern PlayRecords gPlayRecords;

void func_ov005_02367e70(void)
{
    u8 t0 = gPlayRecords.unk_13;
    if (t0 != 255) {
        int t1 = t0 + 1;
        gPlayRecords.unk_13 = t1;
    }
}
