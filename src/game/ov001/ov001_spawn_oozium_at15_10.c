#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

void UnitShowTask_CreateAtTile(int x, int y, int arg);
MapUnit* Unit_Spawn(int x, int y, int type);

/* Puts an Oozium on tile (15, 10): UnitShowTask_CreateAtTile starts a task at the
 * tile (ov006 calls it too, so it keeps its address name), then the unit is
 * created. */
void Ov001_SpawnOoziumAt15_10(void) {
  UnitShowTask_CreateAtTile(15, 10, UNIT_OOZIUM);
  Unit_Spawn(15, 10, UNIT_OOZIUM);
}
