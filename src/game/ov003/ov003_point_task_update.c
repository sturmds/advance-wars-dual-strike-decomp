#include "types.h"
#include "game/ov003_point_task.h"

void Ov003_DrawIconAt(int x, int y);
void Task_ResumeScript(Ov003PointTask* task);

void Ov003_PointTaskUpdate(Ov003PointTask* task) {
  Ov003_DrawIconAt(task->x, task->y);
  if (task->frame != 16) {
    task->frame++;
    return;
  }
  Task_ResumeScript(task);
}
