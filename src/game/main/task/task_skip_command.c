#include "types.h"
#include "game/task.h"

/* Script command that does nothing: moves to the next command. */
BOOL Task_SkipCommand(Task* task) {
  task->pc++;
  return TRUE;
}
