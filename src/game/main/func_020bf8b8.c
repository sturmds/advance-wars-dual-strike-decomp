#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

BOOL UnitMenu_JoinHidden(void);
BOOL UnitMenu_BoardHidden(void);
int func_020dc1a8(void);

s8 func_020bf8b8(void) {
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  RangeMap_Fill(-1);
  MAP_STATE->rangeMap[MAP_STATE->actingUnitDestY][MAP_STATE->actingUnitDestX] =
      0;
  return func_020dc1a8() == 0;
}
