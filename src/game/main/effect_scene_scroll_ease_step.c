#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"
#include "game/math.h"

void Task_ResumeScript(Unk_021694c0Task* task);
void Bg_SetScroll(int layer, int scrollX, int scrollY);

/* Scrolls layer 3 to the next eased value; resumes the script when the
 * duration is reached. */
void EffectScene_ScrollEaseStep(Unk_021694c0Task* task) {
  Bg_SetScroll(3, 0,
               Math_Ease(0, task->easeFrom, task->easeTo, task->easeElapsed++,
                         task->easeDuration));
  if (task->easeElapsed == task->easeDuration) {
    Task_ResumeScript(task);
  }
}
