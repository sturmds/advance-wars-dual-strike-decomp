#include "types.h"

typedef struct Unk_020063e8_0 Unk_020063e8_0;

struct Unk_020063e8_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_020063e8_0 *Task_CreateChild(void *);
extern u8 data_02157264[];

void func_020063e8(int a0)
{
    Unk_020063e8_0 *t0 = Task_CreateChild(data_02157264);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
