#include "types.h"

BOOL EventScript_IsRunning(void);
void Task_ResumeScript(void* task);

void func_0208c158(void* task) {
  if (EventScript_IsRunning() != FALSE) {
    return;
  }
  Task_ResumeScript(task);
}
