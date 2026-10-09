#include "types.h"
#include "game/map_unit.h"

typedef struct TilePos {
    s16 x;
    s16 y;
} TilePos;

BOOL func_020c7bc0(MapUnit *unit);
BOOL func_020c7a3c(MapUnit *unit);
BOOL func_020c78c4(MapUnit *unit, TilePos *tile);

/* Name is a guess from the one caller, AiUnit_MoveToOtherMap, which asks
 * it about each reachable tile. TRUE when func_020c7bc0 accepts the unit,
 * func_020c7a3c does not return 1 for it, and func_020c78c4 accepts the tile. */
BOOL AiUnit_CanUseOtherMapFrom(MapUnit *unit, s16 x, s16 y)
{
    TilePos tile;

    if (!func_020c7bc0(unit)) {
        return FALSE;
    }
    if (func_020c7a3c(unit) == 1) {
        return FALSE;
    }
    tile.x = x;
    tile.y = y;
    return func_020c78c4(unit, &tile) != 0;
}
