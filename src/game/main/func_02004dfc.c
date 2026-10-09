#include "types.h"

typedef struct Unk_02004dfc_0 Unk_02004dfc_0;

struct Unk_02004dfc_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_02004dfc_0 *Task_Create(void *, int);
extern u8 data_021572e4[];

void func_02004dfc(int a0)
{
    Unk_02004dfc_0 *t0 = Task_Create(data_021572e4, 3);
    if ((int)t0 != 0) {
        t0->unk_64 = a0;
    }
}
