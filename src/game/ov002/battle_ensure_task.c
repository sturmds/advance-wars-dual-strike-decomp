#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(const TaskScriptCmd* script);

extern const TaskScriptCmd data_ov002_02360044[];
extern const TaskScriptCmd data_ov002_02360054[];

/* Creates the task of data_ov002_02360054 unless the task of
 * data_ov002_02360044 (made by func_ov002_02350e1c) already runs. */
void Battle_EnsureTask(void) {
  if (Task_FindByScript(data_ov002_02360044) != NULL) {
    return;
  }
  Task_Create(data_ov002_02360054, TASK_LIST(1));
}
