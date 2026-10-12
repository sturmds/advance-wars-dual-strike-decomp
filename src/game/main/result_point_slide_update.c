#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void ResultPoint_DrawPanel(int x, int y, int arg);
void Task_ResumeScript(ResultPointTask* task);

/* Task update: draws the result point sprites, eased 256 pixels to the left
 * over task->value frames. When they have arrived it clears unk_a4 and
 * resumes the task's script. */
void ResultPoint_SlideUpdate(ResultPointTask* task) {
  task->frame++;
  ResultPoint_DrawPanel(8 - Math_Ease(1, 0, 256, task->frame, task->value), 160,
                        data_0229c734->tally);
  if (task->frame != task->value) {
    return;
  }
  data_0229c734->unk_a4 = 0;
  task->frame = 0;
  Task_ResumeScript(task);
}
