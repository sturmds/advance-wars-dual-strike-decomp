#include "types.h"
#include "game/task.h"

BOOL EventScript_IsRunning(void);
void Task_ResumeScript(Task* task);
void func_0204d8c8(void);

/* Once no event script is running: creates the func_0204d8c8 task, then resumes this task. */
void func_0204e6bc(Task* task) {
  if (EventScript_IsRunning() != FALSE) {
    return;
  }
  func_0204d8c8();
  Task_ResumeScript(task);
}
