#include "types.h"
#include "game/task.h"
#include "game/battle_fighter.h"

typedef struct FighterTask {
    u8 pad_00[0x40];
    u16 unk_40;
    u8 unk_42;
} FighterTask;
extern const TaskScriptCmd data_ov002_02369724[];

void func_ov002_0235f3c0(BattleFighter *fighter)
{
    FighterTask *task = (FighterTask *)Task_Create(data_ov002_02369724, TASK_LIST(2));

    task->unk_42 = fighter->side;
    task->unk_40 = fighter->unk_44;
}
