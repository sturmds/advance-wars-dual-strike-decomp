#include "types.h"

typedef struct Unk_020f4658_0 Unk_020f4658_0;

struct Unk_020f4658_0 {
    u8 pad_00[0x8];
    int unk_8;
    int unk_c;
};

extern Unk_020f4658_0 *data_021694c0;

int func_020f4658(int a0)
{
    Unk_020f4658_0 *t0 = data_021694c0;
    if (a0 == t0->unk_8) {
        return 0;
    }
    t0->unk_c = a0;
    return 1;
}
