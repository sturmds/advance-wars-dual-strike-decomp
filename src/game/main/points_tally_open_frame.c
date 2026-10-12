#include "types.h"
#include "game/points_tally_task.h"

void func_0201e6e8(int x, int y, int width, int height);
void Task_ResumeScript(void* task);

/* Script step: draws a frame on layer 1 that grows from column 16 by one
 * tile to each side per frame, to 20 by 8 tiles at row 8 after 10 frames,
 * then resumes the script. */
void PointsTally_OpenFrame(PointsTallyTask* task) {
  s16 left;
  s16 width;

  task->frame++;
  left = 16 - task->frame;
  width = task->frame * 2;
  func_0201e6e8(left, 8, width, 8);
  if (task->frame == 10) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
