#include "types.h"

typedef struct Unk_02290718 Unk_02290718;

struct Unk_02290718 {
    u8 pad_00[0x13];
    u8 unk_13;
};

extern Unk_02290718 data_02290718;

void func_ov005_02367e70(void)
{
    u8 t0 = data_02290718.unk_13;
    if (t0 != 255) {
        int t1 = t0 + 1;
        data_02290718.unk_13 = t1;
    }
}
