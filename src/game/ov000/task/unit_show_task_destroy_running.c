#include "types.h"
#include "game/unit_show_task.h"

extern u8 data_ov000_022db6cc[];

UnitShowTask* Task_FindByScript(void* script);
void UnitShowTask_Destroy(UnitShowTask* task);

/* Destroys the running unit show task, if any. */
void UnitShowTask_DestroyRunning(void) {
  UnitShowTask* task = Task_FindByScript(data_ov000_022db6cc);

  if (task == NULL) {
    return;
  }
  UnitShowTask_Destroy(task);
}
