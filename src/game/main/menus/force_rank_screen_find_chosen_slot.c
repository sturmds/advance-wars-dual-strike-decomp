#include "types.h"
#include "game/force_rank_screen.h"

/* Slot (0 to 3) that holds `skill`, or -1. Only the slots that the Force Rank
 * opens are searched; skill 0 is never found. */
int ForceRankScreen_FindChosenSlot(int skill) {
  int slot;
  int slotCount;

  if (skill == 0) {
    return -1;
  }
  slotCount = gForceRankScreen->rank;
  if (slotCount > 4) {
    slotCount = 4;
  }
  for (slot = 0; slot < slotCount; slot++) {
    if (skill == gForceRankScreen->chosenSkills[slot]) {
      return slot;
    }
  }
  return -1;
}
