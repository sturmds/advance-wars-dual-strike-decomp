#include "types.h"
#include "game/task.h"

void Task_DestroyVisitor(Task* task);
void Task_ForEachByScript(void* script, void (*visit)(Task* task));

/* Destroys every task that was started with this script (Task.script). */
void Task_DestroyByScript(void* script) {
  Task_ForEachByScript(script, Task_DestroyVisitor);
}
