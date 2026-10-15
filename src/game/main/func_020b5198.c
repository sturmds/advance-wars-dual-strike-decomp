#include "types.h"
#include "game/ai/ai_units_in_range.h"

int func_020b5198(void) {
  int t0 = Ai_ScoreReachableEnemyProperties();
  int t1 = Ai_CountTeamArmedGroundUnitsInRange();
  return t0 - t1;
}
