#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Ai_CountEnemyGroundUnitsInRange(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills the range map from (x, y) for the unit in cargo slot 1. TRUE when
 * Ai_CountEnemyGroundUnitsInRange is nonzero and the armed ground units
 * reached number less than its result plus 5. */
BOOL AiSeaTransport_FewArmedUnitsInRange(int x, int y) {
  MapUnit* units = STATE->units;
  int slot = STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];
  int limit;

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  limit = Ai_CountEnemyGroundUnitsInRange();
  if (limit == 0) {
    return FALSE;
  }
  if (Ai_CountTeamArmedGroundUnitsInRange() < limit + 5) {
    return TRUE;
  }
  return FALSE;
}
