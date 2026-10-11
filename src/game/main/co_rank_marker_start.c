#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/task.h"

extern u8 data_0215b004[];

void CoRankMarker_Start(u32 x, u32 y, int matrix)
{
    Unk_02031a14 *task = (Unk_02031a14 *)Task_Create((const TaskScriptCmd *)data_0215b004, TASK_LIST(3));

    task->x = x;
    task->y = y;
    task->matrix = matrix;
}
