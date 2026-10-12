#include "types.h"
#include "game/screen_other_map.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"

u8 Player_GetArmyStyle(int playerIndex);

const char* UnitShowTask_GetPalettePath(UnitShowTask* task) {
  int style = Player_GetArmyStyle(task->owner + 1) - 1;

  if (data_027c027c == data_027c0288.unk_00) {
    return gUnitShowEntries[task->entry].mainPalettePath[style];
  }
  return gUnitShowEntries[task->entry].subPalettePath[style];
}
