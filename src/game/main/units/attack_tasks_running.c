#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(void* script);
extern TaskScriptCmd data_02168328[];
extern TaskScriptCmd data_02168400[];

/* TRUE while either of the two attack animation tasks is running. */
BOOL Attack_TasksRunning(void) {
  if (Task_FindByScript(data_02168400) != NULL ||
      Task_FindByScript(data_02168328) != NULL) {
    return TRUE;
  }
  return FALSE;
}
