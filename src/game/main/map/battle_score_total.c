#include "types.h"
#include "game/Unk_022a7a9c.h"

/* Speed + power + technique, at most 999. The player argument is unused. */
int BattleScore_Total(int player) {
  int score =
      data_022a7a9c.speed + data_022a7a9c.power + data_022a7a9c.technique;

  if (score >= 1000) {
    score = 999;
  }
  return score;
}
