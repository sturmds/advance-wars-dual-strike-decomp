#include "types.h"

typedef struct Unk_020063c0_0 Unk_020063c0_0;

struct Unk_020063c0_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_020063c0_0 *Task_Create(void *, int);
extern u8 data_02157244[];

void func_020063c0(int a0)
{
    Unk_020063c0_0 *t0 = Task_Create(data_02157244, 3);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
