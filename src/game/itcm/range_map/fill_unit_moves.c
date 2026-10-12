#include "types.h"
#include "game/map_unit.h"

int Unit_GetMovePoints(
    MapUnit* unit); /* movement points the unit can use this turn */
/* Fills Unk_02080440_48.rangeMap with the cost to reach each tile from
 * (x, y); -1: out of reach. Oozium ignores the last argument. */
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);

/* Fills the range map with the tiles the unit can move to. */
void RangeMap_FillUnitMoves(MapUnit* unit) {
  RangeMap_FillMoves(unit->x, unit->y, unit->type, Unit_GetMovePoints(unit), 1);
}
