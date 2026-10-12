#include "types.h"
#include "game/cannon_shot_task.h"

void func_020f5888(int x, int y);

/* Step of script data_02168088: calls func_020f5888 on the target tile. */
void CannonShotTask_ViewTarget(CannonShotTask* task) {
  func_020f5888(task->targetX, task->targetY);
}
