#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

void UnitShowTask_CreateAtTile(int x, int y, int arg);
MapUnit *func_020c7d30(int x, int y, int type);

/* Puts an Oozium on tile (9, 12): UnitShowTask_CreateAtTile starts a task at the
 * tile (ov006 calls it too, so it keeps its address name), then the unit is
 * created. */
void Ov001_SpawnOoziumAt9_12(void)
{
    UnitShowTask_CreateAtTile(9, 12, UNIT_OOZIUM);
    func_020c7d30(9, 12, UNIT_OOZIUM);
}
