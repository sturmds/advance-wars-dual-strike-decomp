#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiActingUnit_GetSearchMovement(void);
void AiActingUnit_MarkAttackRange(int movement);
BOOL AiUnit_FindFiringTile(s16* tile);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs the two setup passes, then takes the acting unit's AI target if
 * the range map reaches it and hands it to AiUnit_FindFiringTile. Writes the
 * tile that call leaves in tile to out when it succeeds. */
BOOL func_020a9f9c(s16* out) {
  s16 tile[2];

  AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
  if (STATE->actingUnit->aiTargetX == MAP_UNIT_NO_TARGET) {
    return FALSE;
  }
  if (STATE->rangeMap[STATE->actingUnit->aiTargetY]
                     [STATE->actingUnit->aiTargetX] < 0) {
    return FALSE;
  }
  tile[0] = STATE->actingUnit->aiTargetX;
  tile[1] = STATE->actingUnit->aiTargetY;
  if (!AiUnit_FindFiringTile(tile)) {
    return FALSE;
  }
  out[0] = tile[0];
  out[1] = tile[1];
  return TRUE;
}
