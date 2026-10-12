#include "types.h"
#include "game/task.h"
#include "game/BattleZoomTask.h"

extern const TaskScriptCmd data_ov002_02369684[];

void func_ov002_0235cd64(int shake) {
  BattleZoomTask* task =
      (BattleZoomTask*)Task_Create(data_ov002_02369684, TASK_LIST(2));
  task->shake = shake;
}
