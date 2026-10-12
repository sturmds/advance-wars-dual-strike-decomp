#include "types.h"
#include "game/power_activate_task.h"

void func_020e1acc(int player, int power, void* arg);

/* Script callback of the data_02166f5c task that Power_StartTasks starts:
 * runs the power's activate routine through func_020e1acc. */
void PowerActivateTask_Run(PowerActivateTask* task) {
  func_020e1acc(task->player, task->power, task);
}
