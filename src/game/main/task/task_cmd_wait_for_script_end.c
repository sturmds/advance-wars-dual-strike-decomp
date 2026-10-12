#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(const TaskScriptCmd* script);

/* Script command: holds on this command while any task runs the script in
 * the command's value, then goes on. */
BOOL TaskCmd_WaitForScriptEnd(Task* task) {
  if (Task_FindByScript((const TaskScriptCmd*)task->pc->value) != NULL) {
    return FALSE;
  }
  task->pc++;
  return TRUE;
}
