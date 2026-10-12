#include "types.h"
#include "game/task.h"
#include "game/battle_fighter_task.h"

extern const TaskScriptCmd data_ov002_02369764[];

void Battle_StartFighterTask(BattleCellTask* fighter) {
  BattleFighterTask* task =
      (BattleFighterTask*)Task_Create(data_ov002_02369764, TASK_LIST(2));

  task->side = fighter->side;
  task->scale = fighter->scale;
}
