#include "types.h"

typedef struct Unk_020edb84_0 Unk_020edb84_0;

struct Unk_020edb84_0 {
    u8 pad_00[0x4c];
    int unk_4c;
};

Unk_020edb84_0 *Task_Create(void *, int);
extern u8 data_02168f48[];

void func_020edb84(int a0)
{
    Unk_020edb84_0 *t0 = Task_Create(data_02168f48, 3);
    t0->unk_4c = a0;
}
