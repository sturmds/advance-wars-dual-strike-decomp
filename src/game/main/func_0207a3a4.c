#include "types.h"

typedef struct Unk_0207a3a4_0 Unk_0207a3a4_0;

struct Unk_0207a3a4_0 {
    u8 pad_00[0x34];
    u8 unk_34;
};

extern Unk_0207a3a4_0 *data_0215fe84;

u8 func_0207a3a4(void)
{
    return data_0215fe84->unk_34;
}
