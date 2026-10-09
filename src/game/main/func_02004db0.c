#include "types.h"

typedef struct Unk_02004db0_0 Unk_02004db0_0;

struct Unk_02004db0_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_02004db0_0 *Task_CreateChild(void *);
extern u8 data_021572e4[];

void func_02004db0(int a0)
{
    Unk_02004db0_0 *t0 = Task_CreateChild(data_021572e4);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
