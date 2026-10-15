#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);
int func_020df2f8(u8* unitType);
void RangeMap_SetAdjacentReachable(int x, int y);
int func_020dc080(void);

/* Unit menu entry test: 1 hides the Supply entry. Same as the Repair test,
 * but for a unit type func_020df2f8 accepts and a unit to supply next to the
 * destination. */
s8 UnitMenu_SupplyHidden(void) {
  if (MAP_STATE->unk_1c == 5) {
    return 1;
  }
  if (UnitMenu_JoinHidden() == 0) {
    return 1;
  }
  if (UnitMenu_BoardHidden() == 0) {
    return 1;
  }
  if (func_020df2f8(&MAP_STATE->actingUnit->type) == 0) {
    return 1;
  }
  RangeMap_Fill(-1);
  RangeMap_SetAdjacentReachable(MAP_STATE->actingUnitDestX,
                                MAP_STATE->actingUnitDestY);
  return func_020dc080() == 0;
}
