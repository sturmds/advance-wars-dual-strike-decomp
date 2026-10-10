#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/task.h"

extern u8 data_02159d78[];

void func_02031a14(u32 unk_2c, u32 unk_30, int unk_52)
{
    Unk_02031a14 *task = (Unk_02031a14 *)Task_Create((const TaskScriptCmd *)data_02159d78, TASK_LIST(3));

    task->unk_2c = unk_2c;
    task->unk_30 = unk_30;
    task->unk_52 = unk_52;
}
