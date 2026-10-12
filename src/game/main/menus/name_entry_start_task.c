/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/task.h"
#include "game/name_entry_work.h"

extern const TaskScriptCmd gNameEntryScript[];

/* Starts the name entry screen under parent. maxLength is 8 or 16. */
void NameEntry_StartTask(Task* parent, int maxLength) {
  NameEntryTask* task =
      (NameEntryTask*)Task_CreateChild(gNameEntryScript, parent);

  task->maxLength = maxLength;
}
