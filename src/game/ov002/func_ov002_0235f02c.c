#include "types.h"
#include "game/task.h"
#include "game/battle_fighter_task.h"
#include "game/BattleCellTask.h"

extern const TaskScriptCmd data_ov002_02369744[];

void func_ov002_0235f02c(BattleCellTask* fighter) {
  BattleFighterTask* task =
      (BattleFighterTask*)Task_Create(data_ov002_02369744, TASK_LIST(2));

  task->side = fighter->side;
  task->scale = fighter->scale;
}
