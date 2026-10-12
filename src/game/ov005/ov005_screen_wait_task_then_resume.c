#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_02351dd4.h"

BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void Task_ResumeScript(Ov005Task* task);

/* Script step: once the child task has finished, destroys it and resumes the task script. */
void Ov005Screen_WaitTaskThenResume(Ov005Task* task) {
  Unk_ov005_02351dd4* work = task->work;

  if (BrightnessFade_IsActive(work->waitTask)) {
    return;
  }
  func_020fccb0(work->waitTask);
  Task_ResumeScript(task);
}
