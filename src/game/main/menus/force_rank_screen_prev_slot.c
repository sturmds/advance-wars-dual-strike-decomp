#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* L button: selects the previous skill slot, wrapping around. At most 4 slots
 * exist, min(4, rank); the button does nothing when rank is below 2. */
void ForceRankScreen_PrevSlot(void) {
  int slotCount;

  if (gForceRankScreen->rank < 2) {
    return;
  }
  Sound_PlaySe(0x10002);
  slotCount = gForceRankScreen->rank;
  if (slotCount > 4) {
    slotCount = 4;
  }
  if (gForceRankScreen->selectedSlot != 0) {
    gForceRankScreen->selectedSlot--;
  } else {
    gForceRankScreen->selectedSlot = slotCount - 1;
  }
}
