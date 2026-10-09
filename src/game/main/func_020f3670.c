#include "types.h"

typedef struct Unk_020f3670_0 Unk_020f3670_0;

struct Unk_020f3670_0 {
    u8 pad_00[0x8];
    int unk_8;
};

void func_020f4990(int);
extern Unk_020f3670_0 *data_021694c0;

void func_020f3670(void)
{
    func_020f4990(data_021694c0->unk_8);
}
