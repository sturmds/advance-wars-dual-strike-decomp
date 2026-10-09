#include "types.h"

typedef struct Unk_02004dd4_0 Unk_02004dd4_0;

struct Unk_02004dd4_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_02004dd4_0 *Task_Create(void *, int);
extern u8 data_02157344[];

void func_02004dd4(int a0)
{
    Unk_02004dd4_0 *t0 = Task_Create(data_02157344, 3);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
