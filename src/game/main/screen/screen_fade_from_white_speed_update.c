#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* Task update: adds the speed (unk_64) to the progress (unk_54) and eases the
 * master brightness from white (16) to normal over unk_66 progress units.
 * At the end it resumes the task's script. */
void ScreenFadeFromWhiteSpeed_Update(Task* task) {
  task->unk_54 += task->unk_64;
  if (task->unk_54 >= task->unk_66) {
    task->unk_54 = task->unk_66;
    Task_ResumeScript(task);
  }
  data_027c0280->masterBrightness =
      Math_Ease(0, 16, 0, task->unk_54, task->unk_66);
}
