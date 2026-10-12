#include "types.h"
#include "game/map_unit.h"
#include "game/task.h"

extern const TaskScriptCmd data_021664e0[];

/* Starts the per-unit task (script data_021664e0) under `parent`
 * for a unit that is on the map and not carried inside another unit. */
void LinkPlayerDefeat_StartUnitTask(MapUnit* unit, Task* parent) {
  Task* task;

  if (unit->type == 0) {
    return;
  }
  if (unit->flags & MAP_UNIT_CARRIED) {
    return;
  }
  task = Task_Create(data_021664e0, parent);
  task->unk_4c = unit;
}
