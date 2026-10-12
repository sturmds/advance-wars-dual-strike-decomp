#include "types.h"
#include "game/task.h"
#include "game/ov003_point_task.h"

extern const TaskScriptCmd data_ov003_02364a8c[];

void Ov003_PointTaskCreate(int x, int y) {
  Ov003PointTask* task =
      (Ov003PointTask*)Task_Create(data_ov003_02364a8c, TASK_LIST(3));

  task->x = x;
  task->y = y;
}
