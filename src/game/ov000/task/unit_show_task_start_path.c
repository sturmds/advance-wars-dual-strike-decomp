#include "types.h"
#include "game/task.h"
#include "game/unit_show_task.h"

extern TaskScriptCmd data_ov000_022db6cc[];

UnitShowTask* Task_FindByScript(void* script);

/* Gives the running unit show task a path to walk; does nothing when there is none. */
void UnitShowTask_StartPath(u8* path) {
  UnitShowTask* task = Task_FindByScript(data_ov000_022db6cc);

  if (task == NULL) {
    return;
  }
  UnitShowTask_SetPath(task, path);
}
