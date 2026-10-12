#include "types.h"
#include "game/tag_break_task.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* Step of the power screen script (child task of PowerScreen_Start): eases the
 * alpha blend weight EVA from 16 to 0 over 36 frames, then resumes the script. */
void PowerScreen_EaseOutBlend(TagBreakTask* task) {
  task->frame++;
  data_027c0280->blendEva = Math_Ease(1, 16, 0, task->frame, 36);
  if (task->frame == 36) {
    task->frame = 0;
    Task_ResumeScript((Task*)task);
  }
}
