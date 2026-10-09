#include "types.h"

typedef struct Unk_0200640c_0 Unk_0200640c_0;

struct Unk_0200640c_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_0200640c_0 *Task_CreateChild(void *);
extern u8 data_02157244[];

void func_0200640c(int a0)
{
    Unk_0200640c_0 *t0 = Task_CreateChild(data_02157244);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
