#include "types.h"
#include "game/menu_state.h"

/* Compares the shown HP (1 to 10, 0 when destroyed) with one of the side's
 * five 3-bit steps, which are kept as a halfword at gMenuState + 0x294 + 2 * side.
 * Returns 2 when the step lies below, 1 when it sits on, 0 when above. */
int Battle_CompareHpToCellStep(int side, int shownHp, int index) {
  int step;
  u16* steps;

  if (shownHp == 0) {
    return 0;
  }
  /* Needed to match: the 0x200 is added at run time, then 0x94 in the load. */
  steps = (u16*)((u8*)&gMenuState + side * 2 + 0x200);
  step = ((steps[0x94 / 2] >> (index * 3)) & 7) * 2;
  if (step < shownHp - 1) {
    return 2;
  }
  return shownHp - 1 == step ? 1 : 0;
}
