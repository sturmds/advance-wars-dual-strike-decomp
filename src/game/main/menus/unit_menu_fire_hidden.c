#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern u8 data_022a6b34[];

void RangeMap_FillAttackRange(int x, int y, MapUnit* unit);
BOOL FireTargets_Build(void);

/* Unit menu entry test for the first Fire entry (table at 0x02166050).
 * 0 shows the entry, 1 hides it. It shows when nothing is pending in
 * data_022a6b34, no other unit stands on the destination tile, the unit has
 * a weapon (only a direct one after moving), and FireTargets_Build finds
 * something to fire on in its attack range. */
s8 UnitMenu_FireHidden(void) {
  if (data_022a6b34[0] + data_022a6b34[1] != 0) {
    return 1;
  }
  if (MAP_STATE->mapTiles
          ->unitAt[MAP_STATE->actingUnitDestX +
                   MAP_STATE->mapTiles->rowStart[MAP_STATE->actingUnitDestY]] !=
      0) {
    return 1;
  }
  if (MAP_STATE->actingUnitDestX != MAP_STATE->actingUnitStartX ||
      MAP_STATE->actingUnitDestY != MAP_STATE->actingUnitStartY) {
    if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange !=
        ATTACK_DIRECT) {
      return 1;
    }
  }
  if (Ov000_gUnitStats[MAP_STATE->actingUnit->type].minRange == ATTACK_NONE) {
    return 1;
  }
  RangeMap_Fill(-1);
  RangeMap_FillAttackRange(MAP_STATE->actingUnitDestX,
                           MAP_STATE->actingUnitDestY, MAP_STATE->actingUnit);
  return FireTargets_Build() == 0;
}
