#include "types.h"

typedef struct Unk_0207a148_0 Unk_0207a148_0;

struct Unk_0207a148_0 {
    u8 pad_00[0x1c];
    int unk_1c;
};

extern Unk_0207a148_0 *data_0215fe84;

int func_0207a148(void)
{
    Unk_0207a148_0 *t0 = data_0215fe84;
    if (t0->unk_1c == 1) {
        t0->unk_1c = 0;
        return 1;
    }
    return 0;
}
