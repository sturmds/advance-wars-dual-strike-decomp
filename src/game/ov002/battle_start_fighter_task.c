#include "types.h"
#include "game/task.h"
#include "game/FighterTask.h"

extern const TaskScriptCmd data_ov002_02369764[];

void Battle_StartFighterTask(BattleFighter *fighter)
{
    FighterTask *task = (FighterTask *)Task_Create(data_ov002_02369764, TASK_LIST(2));

    task->side = fighter->side;
    task->scale = fighter->unk_44;
}
