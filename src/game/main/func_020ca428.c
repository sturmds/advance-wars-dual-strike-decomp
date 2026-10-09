#include "types.h"

typedef struct Unk_020ca428_0 Unk_020ca428_0;

struct Unk_020ca428_0 {
    u8 pad_00[0x64];
    u16 unk_64;
};

Unk_020ca428_0 *Task_Create(void *, int);
extern u8 data_02166538[];
int Map_IsCoSelectPhase(void *);

int func_020ca428(void)
{
    Unk_020ca428_0 *t0 = Task_Create(data_02166538, 3);
    int t1 = Map_IsCoSelectPhase(t0);
    t0->unk_64 = t1;
    return t1;
}
