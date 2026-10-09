#include "types.h"

typedef struct Unk_02290718 Unk_02290718;

struct Unk_02290718 {
    u8 pad_00[0x4];
    int unk_4;
};

extern Unk_02290718 data_02290718;

void func_02017c88(int a0)
{
    if ((u32)data_02290718.unk_4 >= (u32)a0) {
        data_02290718.unk_4 = a0;
    }
}
