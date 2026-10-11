#include "types.h"
#include "game/task.h"
#include "game/battle_fighter.h"
#include "game/BattleCellTask.h"

typedef struct FighterTask {
    u8 pad_00[0x40];
    u16 unk_40;
    u8 unk_42;
} FighterTask;
extern const TaskScriptCmd data_ov002_02369744[];

void func_ov002_0235f02c(BattleCellTask *fighter)
{
    FighterTask *task = (FighterTask *)Task_Create(data_ov002_02369744, TASK_LIST(2));

    task->unk_42 = fighter->side;
    task->unk_40 = fighter->scale;
}
