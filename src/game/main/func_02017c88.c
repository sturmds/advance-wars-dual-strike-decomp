#include "types.h"

typedef struct PlayRecords PlayRecords;

struct PlayRecords {
    u8 pad_00[0x4];
    int unk_4;
};

extern PlayRecords gPlayRecords;

void func_02017c88(int a0)
{
    if ((u32)gPlayRecords.unk_4 >= (u32)a0) {
        gPlayRecords.unk_4 = a0;
    }
}
