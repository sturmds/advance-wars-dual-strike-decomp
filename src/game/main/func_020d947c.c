#include "types.h"

typedef struct Unk_020d947c Unk_020d947c;

struct Unk_020d947c {
    u8 pad_00[0x64];
    s16 unk_64;
    s16 unk_66;
};

void func_020f5888(int, int);

void func_020d947c(Unk_020d947c *a0)
{
    func_020f5888(a0->unk_64, a0->unk_66);
}
