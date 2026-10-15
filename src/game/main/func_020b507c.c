#include "types.h"
#include "game/ai/ai_units_in_range.h"

int func_020b507c(void) {
  int t0 = Ai_CountTeamFootUnitsInRange();
  int t1 = Ai_CountTeamArmedGroundUnitsInRange();
  return t0 - t1;
}
