#include "types.h"
#include "game/task.h"

typedef struct Unk_0235cd64_0 Unk_0235cd64_0;

struct Unk_0235cd64_0 {
    u8 pad_00[0x50];
    u16 unk_50;
};

extern const TaskScriptCmd data_ov002_02369684[];

void func_ov002_0235cd64(int a0)
{
    Unk_0235cd64_0 *t0 = (Unk_0235cd64_0 *)Task_Create(data_ov002_02369684, TASK_LIST(2));
    t0->unk_50 = a0;
}
