#include "types.h"

extern u8 gSavedArmyColours[5];

/* Sets slot i of gSavedArmyColours back to colour i. */
void SavedArmyColours_Reset(void) {
  int i;

  for (i = 0; i < 5; i++) {
    gSavedArmyColours[i] = i;
  }
}
