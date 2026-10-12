#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);
extern const TaskScriptCmd data_021601a0[];

/* Resumes the task once no task is running the script at data_021601a0. */
void func_0207e5fc(void* task) {
  if (Task_FindByScript(data_021601a0) != NULL) {
    return;
  }
  Task_ResumeScript(task);
}
