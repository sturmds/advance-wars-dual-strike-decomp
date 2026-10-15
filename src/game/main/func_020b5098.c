#include "types.h"
#include "game/ai/ai_units_in_range.h"

int func_020b5098(void) {
  int t0 = Ai_CountTeamArmedGroundUnitsInRange();
  int t1 = Ai_CountTeamFootUnitsInRange();
  return (t0 < t1) ? 1 : 0;
}
