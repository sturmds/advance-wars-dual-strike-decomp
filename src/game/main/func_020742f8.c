#include "types.h"

typedef struct Unk_020742f8 Unk_020742f8;

struct Unk_020742f8 {
    u8 pad_00[0x2c];
    u8 unk_2c;
};

int func_020742f8(Unk_020742f8 *a0)
{
    u8 t0 = a0->unk_2c;
    if (t0 != 0) {
        int t1 = t0 - 1;
        a0->unk_2c = t1;
        return 1;
    }
    return 0;
}
