#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int GetUnitMaxRange(int player, int unitType);
/* Writes value into every range map tile within radius steps of (x, y). */
void RangeMap_FillDiamondU16(u16 x, u16 y, u16 radius, s8 value);

/* Marks the tiles a unit standing at (x, y) can fire on: 0 within its
 * maximum range, then -1 again inside its minimum range. */
void RangeMap_FillAttackRange(int x, int y, MapUnit* unit) {
  RangeMap_FillDiamondU16(
      x, y, GetUnitMaxRange(MAP_UNIT_OWNER(MAP_STATE, unit), unit->type), 0);
  RangeMap_FillDiamondU16(x, y, Ov000_gUnitStats[unit->type].minRange - 1, -1);
}
