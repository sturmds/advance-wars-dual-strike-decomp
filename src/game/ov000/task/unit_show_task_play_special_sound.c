#include "types.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"

void UnitShowTask_PlaySpecialSound(UnitShowTask* task) {
  if (gUnitShowEntries[task->entry].sound == SHOW_SOUND_SPECIAL) {
    UnitShowTask_PlayStepSound(task);
  }
}
