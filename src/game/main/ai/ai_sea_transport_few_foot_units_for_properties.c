#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int AiUnit_GetUnloadMovement(MapUnit* unit);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Ai_CountTeamFootUnitsInRange(void);
int Ai_ScoreReachableEnemyProperties(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills the range map from (x, y) for the unit in cargo slot 1. TRUE when
 * the team's foot units it reaches number less than the value of the enemy
 * properties it reaches plus 2. */
BOOL AiSeaTransport_FewFootUnitsForProperties(int x, int y) {
  MapUnit* units = STATE->units;
  int slot = STATE->actingUnit->cargo[1];
  MapUnit* carried = &units[slot];
  int footUnits;

  RangeMap_FillMoves(x, y, carried->type, AiUnit_GetUnloadMovement(carried), 0);
  footUnits = Ai_CountTeamFootUnitsInRange();
  return footUnits < Ai_ScoreReachableEnemyProperties() + 2;
}
