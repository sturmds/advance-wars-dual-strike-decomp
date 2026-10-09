#include "types.h"

typedef struct Unk_02004cf4_0 Unk_02004cf4_0;

struct Unk_02004cf4_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_02004cf4_0 *Task_CreateChild(void *);
extern u8 data_02157224[];

void func_02004cf4(int a0)
{
    Unk_02004cf4_0 *t0 = Task_CreateChild(data_02157224);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
