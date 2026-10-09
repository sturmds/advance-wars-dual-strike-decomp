#include "types.h"

typedef struct Unk_020d8f14 Unk_020d8f14;

struct Unk_020d8f14 {
    u8 pad_00[0x2c];
    int unk_2c;
    int unk_30;
};

void func_020d8e9c(int, int, int);

void func_020d8f14(Unk_020d8f14 *a0)
{
    func_020d8e9c(a0->unk_2c, a0->unk_30, 3);
}
