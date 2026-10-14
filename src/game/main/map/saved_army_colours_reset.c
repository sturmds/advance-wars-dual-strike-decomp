#include "types.h"
#include "game/map_header.h"

extern u8 gSavedArmyColours[];

/* Sets slot i of gSavedArmyColours back to colour i. */
void SavedArmyColours_Reset(void) {
  int i;

  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    gSavedArmyColours[i] = i;
  }
}
