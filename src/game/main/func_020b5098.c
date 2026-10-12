#include "types.h"

int Ai_CountTeamArmedGroundUnitsInRange(void);
int Ai_CountTeamFootUnitsInRange(void);

int func_020b5098(void) {
  int t0 = Ai_CountTeamArmedGroundUnitsInRange();
  int t1 = Ai_CountTeamFootUnitsInRange();
  return (t0 < t1) ? 1 : 0;
}
