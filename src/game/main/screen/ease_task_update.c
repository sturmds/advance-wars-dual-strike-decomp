#include "types.h"
#include "game/task.h"
#include "game/math.h"

void Task_ResumeScript(Task* task);
void func_020fbb34(int value);

/* One frame of a value ease: sets the value func_020fbb34 stores to the ease
 * from unk_64 to unk_66 at frame unk_44 of unk_54, and resumes the task's
 * script after the last frame. */
void EaseTask_Update(Task* task) {
  func_020fbb34(
      Math_Ease(0, task->unk_64, task->unk_66, task->unk_44, task->unk_54));
  task->unk_44++;
  if (task->unk_44 > task->unk_54) {
    Task_ResumeScript(task);
  }
}
