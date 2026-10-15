#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/task.h"
#include "game/unit_stats.h"

void TileEffectTask_Create(int x, int y, int effect, Task* parent);
void func_020c7388(int unitIndex);
void func_020c8888(void);

/* Task step for the unit in task->unk_4c: starts the unit type's effect on
 * its tile (types with effect 24 get one by army colour), removes the unit
 * as destroyed and refreshes the map. */
void UnitDestroyTask_PlayEffectAndRemove(Task* task) {
  MapUnit* unit = task->unk_4c;
  int effect = Ov000_gUnitStats[unit->type].unk_1d;

  if (effect == 24) {
    switch (MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unit)].armyColour) {
      case 0:
      case 1:
        effect = 31;
        break;
      case 2:
        effect = 32;
        break;
      case 3:
        effect = 33;
        break;
      case 4:
        effect = 34;
        break;
      case 5:
        effect = 35;
        break;
    }
  }
  TileEffectTask_Create(unit->x, unit->y, effect, task);
  func_020c7388(unit - MAP_STATE->units);
  func_020c8888();
}
