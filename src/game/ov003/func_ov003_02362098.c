#include "types.h"

typedef struct Unk_02362098_0 Unk_02362098_0;

struct Unk_02362098_0 {
    u8 pad_00[0x1db];
    u8 unk_1db;
};

extern Unk_02362098_0 *data_ov003_02364928;

int func_ov003_02362098(int a0)
{
    return (a0 != ((u32)(data_ov003_02364928->unk_1db << 25) >> 25)) ? 1 : 0;
}
