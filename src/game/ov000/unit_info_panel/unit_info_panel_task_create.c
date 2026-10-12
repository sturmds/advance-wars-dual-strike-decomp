#include "types.h"
#include "game/script_slot.h"
#include "game/task.h"

extern ScriptCommand data_ov000_0234f30c[];
extern TaskScriptCmd data_ov000_0234f33c[];

int SceneMode_IsCurrent(int a0);
s8 ScriptSlot_Find(const ScriptCommand* script);

/* Makes the panel's task, but only when the panel's script is running. */
void UnitInfoPanel_CreateTask(void) {
  if (SceneMode_IsCurrent(5) == 0) {
    return;
  }
  if (ScriptSlot_Find(data_ov000_0234f30c) == -1) {
    return;
  }
  Task_Create(data_ov000_0234f33c, TASK_LIST(3));
}
