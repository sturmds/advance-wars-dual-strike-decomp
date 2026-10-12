#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define FAR_AWAY \
  110 /* movement of the fill, large enough to reach the whole map */

/* Fills the range map with the acting unit's moves from its own tile. If
 * the unit has an AI target (MAP_UNIT_NO_TARGET means none) that the fill
 * reached, writes the target's x and y to out and returns TRUE. */
BOOL Ai_FindReachableAiTarget(s16* out) {
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, FAR_AWAY, 0);
  if (STATE->actingUnit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  if (STATE->rangeMap[STATE->actingUnit->aiTargetY]
                     [STATE->actingUnit->aiTargetX] < 0) {
    return FALSE;
  }
  out[0] = STATE->actingUnit->aiTargetX;
  out[1] = STATE->actingUnit->aiTargetY;
  return TRUE;
}
