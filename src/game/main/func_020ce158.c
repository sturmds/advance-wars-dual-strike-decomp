#include "types.h"

typedef struct Unk_020ce158_0 Unk_020ce158_0;

struct Unk_020ce158_0 {
    u8 pad_00[0x68];
    u16 unk_68;
};

Unk_020ce158_0 *Task_Create(void *, int);
extern u8 data_021666b0[];

void func_020ce158(int a0)
{
    Unk_020ce158_0 *t0 = Task_Create(data_021666b0, 3);
    t0->unk_68 = a0;
}
