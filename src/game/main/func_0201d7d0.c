#include "types.h"

typedef struct Unk_0201d7d0_0 Unk_0201d7d0_0;

struct Unk_0201d7d0_0 {
    u8 pad_00[0x130];
    int unk_130;
};

void TransferQueue_Add(void *, int, int);
extern u8 data_02297be0[];
extern Unk_0201d7d0_0 *data_027c0280;

void func_0201d7d0(void)
{
    TransferQueue_Add(data_02297be0, data_027c0280->unk_130 + 0x1f000, 0x180);
}
