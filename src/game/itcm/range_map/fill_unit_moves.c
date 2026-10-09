#include "types.h"
#include "game/map_unit.h"

int func_020c67c8(MapUnit *unit); /* movement points the unit can use this turn */
/* Fills Unk_02080440_48.unk_d0 with the cost to reach each tile from
 * (x, y); -1: out of reach. Oozium ignores the last argument. */
void func_01ffb614(int x, int y, int unitType, s16 movement, int arg4);

/* Fills the range map with the tiles the unit can move to. */
void RangeMap_FillUnitMoves(MapUnit *unit)
{
    func_01ffb614(unit->x, unit->y, unit->type, func_020c67c8(unit), 1);
}
