#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"
#include "game/math.h"

extern Unk_022a7b80 data_022a7b80;

/* Frames left of the time budget (data_022a7b80.budgetLeft) after the seconds
 * player 1 has used on this map; 0 when the budget is used up. */
u32 Survival_GetTimeLeft(void) {
  u32 used = HwDiv32(
      data_027c027c->unk_48[data_027c0284]->players[1].playTimeFrames, 60);
  u32 budget = HwDiv32(data_022a7b80.budgetLeft, 60);

  if (budget < used) {
    return 0;
  }
  return (budget - used) * 60;
}
