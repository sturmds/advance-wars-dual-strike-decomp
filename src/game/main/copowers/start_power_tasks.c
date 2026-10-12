#include "types.h"
#include "game/task.h"
#include "game/power_activate_task.h"

extern const TaskScriptCmd data_02166f24[];
extern const TaskScriptCmd data_02166f5c[];

void Power_StartTasks(int player, int power, BOOL startSecond) {
  PowerActivateTask* task =
      (PowerActivateTask*)Task_Create(data_02166f5c, TASK_LIST(3));
  PowerActivateTask* child;

  task->player = player;
  task->power = power;
  if (startSecond) {
    child = (PowerActivateTask*)Task_CreateChild(data_02166f24, (Task*)task);
    child->player = player;
    child->power = power;
  }
}
