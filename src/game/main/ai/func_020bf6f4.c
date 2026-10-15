#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/ai.h"

BOOL UnitMenu_JoinHidden(void);
BOOL UnitMenu_BoardHidden(void);

/* After the two checks of UnitMenu_JoinHidden and UnitMenu_BoardHidden pass, clears the
 * range map, marks the acting unit's destination tile as reachable (0) and
 * returns whether no silo is reachable from there. Returns 1 as soon as
 * either check fails. The meaning of the checks is not known yet. */
s8 func_020bf6f4(void) {
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  RangeMap_Fill(-1);
  MAP_STATE->rangeMap[MAP_STATE->actingUnitDestY][MAP_STATE->actingUnitDestX] =
      0;
  return AiFoot_ListReachableSilos() == 0;
}
