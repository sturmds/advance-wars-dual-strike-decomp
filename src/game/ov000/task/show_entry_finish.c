#include "types.h"
#include "game/task.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"

extern TaskScriptCmd data_ov000_022db70c[];

s8 Task_DestroyByScript(const void* script);

void ShowEntry_Finish(UnitShowTask* task) {
  if (gUnitShowEntries[task->entry].sound != SHOW_SOUND_SPECIAL) {
    Task_DestroyByScript(data_ov000_022db70c);
    UnitShowTask_PlayStepSound(task);
    return;
  }
  UnitShowTask_PlayStepSound(task);
}
