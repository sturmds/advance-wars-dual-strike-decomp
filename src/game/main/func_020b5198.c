#include "types.h"

int Ai_ScoreReachableEnemyProperties(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

int func_020b5198(void) {
  int t0 = Ai_ScoreReachableEnemyProperties();
  int t1 = Ai_CountTeamArmedGroundUnitsInRange();
  return t0 - t1;
}
