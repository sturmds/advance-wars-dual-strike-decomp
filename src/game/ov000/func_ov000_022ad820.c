#include "types.h"

typedef struct Unk_022ad820 Unk_022ad820;

struct Unk_022ad820 {
    u8 pad_00[0x35];
    u8 unk_35;
};

int func_020cfd30(void);

int func_ov000_022ad820(Unk_022ad820 *a0)
{
    int t0 = func_020cfd30();
    if (t0 == 0) {
        a0->unk_35 = 3;
        return 3;
    }
    return t0;
}
