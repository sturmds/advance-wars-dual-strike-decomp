#include "types.h"

typedef struct Unk_020e3e2c Unk_020e3e2c;

struct Unk_020e3e2c {
    u8 pad_00[0x2c];
    u8 unk_2c;
};

void func_020e1be0(int);

void func_020e3e2c(Unk_020e3e2c *a0)
{
    func_020e1be0(a0->unk_2c);
}
