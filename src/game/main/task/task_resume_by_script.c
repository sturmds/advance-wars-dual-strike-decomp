#include "types.h"
#include "game/task.h"

void Task_ResumeScriptVisitor(Task* task);
void Task_ForEachByScript(void* script, void (*visit)(Task* task));

/* Resumes every task that was started with this script (Task.script). */
void Task_ResumeByScript(void* script) {
  Task_ForEachByScript(script, Task_ResumeScriptVisitor);
}
