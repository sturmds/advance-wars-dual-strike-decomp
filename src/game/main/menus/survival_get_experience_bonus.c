#include "types.h"
#include "game/Unk_022a7b80.h"
#include "game/math.h"

extern Unk_022a7b80 data_022a7b80;

/* Experience bonus for the mode variant. Modes 0 to 2 turn what is left of
 * the budget (data_022a7b80.budgetLeft) into points: half a point per second,
 * 10 per day, 1 per 200 funds. The other modes give 5 for the first map
 * done, 10 for the second, and so on, added up, at most 9999. */
u32 Survival_GetExperienceBonus(void) {
  int bonus;
  int k;
  int mapsDone;

  switch (data_022a7b80.mode) {
    case 0:
      return (u32)HwDiv32(data_022a7b80.budgetLeft, 60) >> 1;
    case 2:
      return data_022a7b80.budgetLeft * 10;
    case 1:
      return HwDiv32(data_022a7b80.budgetLeft, 200);
    default:
      bonus = 0;
      mapsDone = data_022a7b80.mapIndex;
      /* needed to match: k < mapsDone + 1, not k <= mapsDone */
      for (k = 1; k < mapsDone + 1; k++) {
        bonus += k * 5;
      }
      if (bonus > 9999) {
        bonus = 9999;
      }
      return bonus;
  }
}
