#include "types.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

/* What the map just played took from the run's budget (data_022a7b80.budgetLeft),
 * for the mode variant: time in frames, rounded down to whole seconds
 * (modes 0 and 3), funds spent (1 and 4) or days taken (2 and 5). Player 1
 * is the human player. */
int Survival_GetMapCost(int mode) {
  int value = 0;

  switch (mode) {
    case 0:
    case 3:
      value = HwDiv32(MAP_STATE->players[1].playTimeFrames, 60) * 60;
      break;
    case 1:
    case 4:
      value = MAP_STATE->players[1].unk_04;
      break;
    case 2:
    case 5:
      value = MAP_STATE->day - 1;
      break;
  }
  return value;
}
