#include "types.h"
#include "game/task.h"

int ScriptSlot_Find(const void* script);

/* Script command: holds on this command while an event script slot runs the
 * script in the command's value, then goes on. */
BOOL TaskCmd_WaitForEventScriptEnd(Task* task) {
  if (ScriptSlot_Find((const void*)task->pc->value) != -1) {
    return FALSE;
  }
  task->pc++;
  return TRUE;
}
