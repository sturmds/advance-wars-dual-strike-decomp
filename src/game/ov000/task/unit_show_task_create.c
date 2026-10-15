#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"

UnitShowTask* UnitShowTask_Spawn(int x, int y, int player, int type);

/* Marks the unit selected and locked, then makes its show task. The third
 * argument is the unit's slot divided by 64: its owner's player slot minus 1. */
UnitShowTask* UnitShowTask_Create(MapUnit* unit) {
  UnitShowTask* task;

  unit->flags |= MAP_UNIT_SELECTED | MAP_UNIT_LOCKED;
  task = UnitShowTask_Spawn(unit->x, unit->y,
                            (u32)((unit - MAP_STATE->units) << 10) >> 16,
                            unit->type);
  if (task == NULL) {
    return NULL;
  }
  task->unit = unit;
  return task;
}
