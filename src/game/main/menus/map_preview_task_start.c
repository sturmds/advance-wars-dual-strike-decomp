#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(void* script);
extern TaskScriptCmd data_02166e14[];

/* Starts the map preview task unless one is running. It keeps the
 * character data address in unk_4c. Returns whether it started one. */
BOOL MapPreviewTask_Start(void* charData) {
  Task* task;

  if (Task_FindByScript(data_02166e14) != NULL) {
    return FALSE;
  }
  task = Task_Create(data_02166e14, TASK_LIST(3));
  task->unk_4c = charData;
  return TRUE;
}
