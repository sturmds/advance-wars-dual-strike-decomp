#include "types.h"
#include "game/task.h"

int ScriptSlot_StartGetIndex(const void* script, int arg);

/* Script command: starts the event script in the command's value in a free
 * slot, with the low byte of the task's unk_24 as the argument, then goes
 * on. */
BOOL TaskCmd_StartEventScript(Task* task) {
  ScriptSlot_StartGetIndex((const void*)task->pc->value, task->unk_24 & 0xff);
  task->pc++;
  return TRUE;
}
