#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/object_heal_task.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_AddTileOffset(int* x, int* y, int list, int index);
int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);
int Unit_Repair(MapUnit* unit, s16 steps, BOOL payCost);
void func_020c4018(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the object heal script: every Black Hole unit on a tile in reach
 * of the object gets healSteps shown HP, ammo and fuel for free. Oozium
 * (unit class "all") is left out. */
void ObjectHealTask_RepairUnits(ObjectHealTask* task) {
  int x;
  int y;
  MapUnit* unit;

  for (;;) {
    x = task->tileX;
    y = task->tileY;
    if (!Map_AddTileOffset(&x, &y, task->offsetList, task->offsetIndex++)) {
      break;
    }
    if (x < 0 || x >= STATE->unk_1c4->mapWidth || y < 0 ||
        y >= STATE->unk_1c4->mapHeight) {
      continue;
    }
    unit = &STATE->units[STATE->mapTiles
                             ->unk_508[x + STATE->mapTiles->rowStart[y]]];
    if (unit == NULL) {
      continue;
    }
    if (STATE->players[MAP_UNIT_OWNER(STATE, unit)].armyColour !=
        ARMY_BLACK_HOLE) {
      continue;
    }
    if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL) {
      continue;
    }
    Unit_Repair(unit, task->healSteps, FALSE);
    Unit_Rearm(unit, FALSE);
    Unit_Refuel(unit, FALSE);
  }
  func_020c4018();
}
