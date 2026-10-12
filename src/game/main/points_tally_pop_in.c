#include "types.h"
#include "game/points_tally_task.h"

void PointsTally_Draw(int x, int y, int added, int total, int popFrame);
void func_0204cc54(void);
void Task_ResumeScript(void* task);

/* Script step: draws the points box at y 88 with the total popping in,
 * one frame further each call. After task->frames frames it adds and
 * selects the button func_0204cc54 makes, then resumes the script. */
void PointsTally_PopIn(PointsTallyTask* task) {
  task->frame++;
  PointsTally_Draw(56, 88, task->added, task->total, task->frame);
  if (task->frame != task->frames) {
    return;
  }
  task->frame = 0;
  func_0204cc54();
  Task_ResumeScript(task);
}
