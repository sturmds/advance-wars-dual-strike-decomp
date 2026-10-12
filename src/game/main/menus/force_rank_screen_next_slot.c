#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* R button: selects the next skill slot, wrapping around. */
void ForceRankScreen_NextSlot(void) {
  int slotCount;

  if (gForceRankScreen->rank < 2) {
    return;
  }
  Sound_PlaySe(0x10002);
  slotCount = gForceRankScreen->rank;
  if (slotCount > 4) {
    slotCount = 4;
  }
  if (gForceRankScreen->selectedSlot < slotCount - 1) {
    gForceRankScreen->selectedSlot++;
  } else {
    gForceRankScreen->selectedSlot = 0;
  }
}
