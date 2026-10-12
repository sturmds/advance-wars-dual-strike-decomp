#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when unitA goes on the right in the battle scene: it stands further
 * right on the map, or in the same column and its owner's HQ is further right. */
BOOL BattleScene_IsUnitOnRight(MapUnit* unitA, MapUnit* unitB) {
  if (unitA->x > unitB->x) {
    return TRUE;
  }
  if (unitA->x == unitB->x) {
    if ((STATE->players[MAP_UNIT_OWNER(STATE, unitA)].hqX & 0x7f) >
        (STATE->players[MAP_UNIT_OWNER(STATE, unitB)].hqX & 0x7f)) {
      return TRUE;
    }
  }
  return FALSE;
}
