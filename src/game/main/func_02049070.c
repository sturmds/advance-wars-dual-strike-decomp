#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/task.h"

extern u8 data_0215b004[];

void func_02049070(u32 unk_2c, u32 unk_30, int unk_52)
{
    Unk_02031a14 *task = (Unk_02031a14 *)Task_Create((const TaskScriptCmd *)data_0215b004, TASK_LIST(3));

    task->unk_2c = unk_2c;
    task->unk_30 = unk_30;
    task->unk_52 = unk_52;
}
