#include "types.h"
#include "game/math.h"

/* CO rank for `exp` experience points: one rank per 1000, from 0 to 100. */
int ExperienceToRank(int exp) {
  int rank;

  if (exp < 0) {
    return 0;
  }
  rank = HwDiv32(exp, 1000);
  if (rank >= 100) {
    rank = 100;
  }
  return rank;
}
