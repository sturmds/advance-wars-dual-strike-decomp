#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int GetUnitMaxRange(int player, int unitType);
/* Same diamond fill as RangeMap_FillDiamondU16, with s16 coordinates and abs(). */
void RangeMap_FillDiamond(int x, int y, s16 radius, s8 value);

/* Writes 1 into the range map tiles a unit standing at (x, y) can fire on,
 * then -1 inside its minimum range. Units with maximum range 1 skip the
 * second pass. */
void RangeMap_MarkAttackRange(int x, int y, MapUnit* unit) {
  RangeMap_FillDiamond(
      x, y, GetUnitMaxRange(MAP_UNIT_OWNER(MAP_STATE, unit), unit->type), 1);
  if (GetUnitMaxRange(MAP_UNIT_OWNER(MAP_STATE, unit), unit->type) == 1) {
    return;
  }
  RangeMap_FillDiamond(x, y, Ov000_gUnitStats[unit->type].minRange - 1, -1);
}
