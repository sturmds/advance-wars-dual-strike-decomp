#include "types.h"

BOOL EventScript_IsRunning(void);
void Task_ResumeScript(void* task);

/* Resumes the task once no event script is running. */
void Task_ResumeWhenScriptIdle(void* task) {
  if (EventScript_IsRunning() != FALSE) {
    return;
  }
  Task_ResumeScript(task);
}
