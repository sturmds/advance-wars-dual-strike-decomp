#include "types.h"
#include "game/task.h"

void* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(void* task);
extern const TaskScriptCmd data_0216737c[];

void func_020d6a64(void* task) {
  if (Task_FindByScript(data_0216737c) != NULL) {
    return;
  }
  Task_ResumeScript(task);
}
