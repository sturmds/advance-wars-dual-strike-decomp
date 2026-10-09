#include "types.h"

typedef struct Unk_0207c044_0 Unk_0207c044_0;

struct Unk_0207c044_0 {
    u8 pad_00[0x2c];
    int unk_2c;
};

Unk_0207c044_0 *Task_Create(void *, int, int);
extern u8 data_0215ff9c[];

void func_0207c044(int a0, int a1)
{
    Unk_0207c044_0 *t0 = Task_Create(data_0215ff9c, a0, a0);
    t0->unk_2c = a1;
}
