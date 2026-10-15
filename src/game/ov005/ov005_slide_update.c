#include "types.h"
#include "game/task.h"
#include "game/ov005_slide_task.h"
#include "game/math.h"

void Task_ResumeScript(Ov005SlideTask* task);

/* Step 1 eases the entry's pos from `from` to `to`; step 2 resumes the parent
 * script if asked and clears `busy`. */
void Ov005Slide_Update(Ov005SlideTask* task) {
  int index = task->index;
  Ov005SlideEntry* entries = task->entries;

  switch (task->step) {
    case 0:
      task->step++;
    case 1:
      entries[index].pos =
          Math_Ease(0, task->from, task->to, task->frame, task->frames);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step++;
      }
      return;
    case 2:
      if (task->resumeScript != 0) {
        Task_ResumeScript(task);
      }
      task->busy = 0;
      return;
  }
}
