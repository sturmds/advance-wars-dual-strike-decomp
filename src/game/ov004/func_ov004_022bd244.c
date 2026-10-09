#include "types.h"

typedef struct Unk_022bd244 Unk_022bd244;

struct Unk_022bd244 {
    u8 pad_00[0x64];
    int unk_64;
};

void func_0202244c(int);

void func_ov004_022bd244(Unk_022bd244 *a0)
{
    func_0202244c(a0->unk_64);
}
