#include "types.h"
#include "game/task.h"

/* Script of (opcode, argument) pairs. */
extern const TaskScriptCmd gExMachinaScript[];

/* Von Bolt's Super CO Power. Without a parent task the script goes on
 * top-level task list 3. */
void ExMachina(void* parent) {
  if (parent != NULL) {
    Task_CreateChild(gExMachinaScript, parent);
  } else {
    Task_Create(gExMachinaScript, TASK_LIST(3));
  }
}
