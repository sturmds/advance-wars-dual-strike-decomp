#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"
#include "game/math.h"

void Task_JumpToLabel(Task* task, int label);

/* Task step: resets the frame counter and spreads the letters' start frames
 * over 0 to 48, each the mean of two ease curves. With unk_58 zero it jumps
 * to script label 0 instead. */
void ModeFontTask_InitLetterStarts(ModeFontTask* task) {
  int i;

  task->frame = 0;
  if (task->unk_58 == 0) {
    Task_JumpToLabel((Task*)task, 0);
    return;
  }
  for (i = 0; i < task->length; i++) {
    task->letterStart[i] = (Math_Ease(0, 0, 48, i, task->length - 1) +
                            Math_Ease(4, 0, 48, i, task->length - 1)) >>
                           1;
  }
}
