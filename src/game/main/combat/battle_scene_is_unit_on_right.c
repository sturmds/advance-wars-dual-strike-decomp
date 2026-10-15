#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when unitA goes on the right in the battle scene: it stands further
 * right on the map, or in the same column and its owner's HQ is further right. */
BOOL BattleScene_IsUnitOnRight(MapUnit* unitA, MapUnit* unitB) {
  if (unitA->x > unitB->x) {
    return TRUE;
  }
  if (unitA->x == unitB->x) {
    if ((MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unitA)].hqX & 0x7f) >
        (MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unitB)].hqX & 0x7f)) {
      return TRUE;
    }
  }
  return FALSE;
}
