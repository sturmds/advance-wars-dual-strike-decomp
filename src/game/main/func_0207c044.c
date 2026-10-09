#include "types.h"
#include "game/task.h"

typedef struct Unk_0207c044_0 Unk_0207c044_0;

struct Unk_0207c044_0 {
    u8 pad_00[0x2c];
    int unk_2c;
};

extern const TaskScriptCmd data_0215ff9c[];

void func_0207c044(void *parentOrList, int a1)
{
    Unk_0207c044_0 *t0 = (Unk_0207c044_0 *)Task_Create(data_0215ff9c, parentOrList);
    t0->unk_2c = a1;
}
