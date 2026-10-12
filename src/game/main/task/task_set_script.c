#include "types.h"
#include "game/task.h"

/* Starts script from its first command (pc holds the next command)
 * and clears update so the script runs on the next frame. */
void Task_SetScript(Task* task, void* script) {
  task->scriptStart = script;
  task->pc = script;
  task->update = NULL;
}
