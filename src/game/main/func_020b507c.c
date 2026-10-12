#include "types.h"

int Ai_CountTeamFootUnitsInRange(void);
int Ai_CountTeamArmedGroundUnitsInRange(void);

int func_020b507c(void) {
  int t0 = Ai_CountTeamFootUnitsInRange();
  int t1 = Ai_CountTeamArmedGroundUnitsInRange();
  return t0 - t1;
}
