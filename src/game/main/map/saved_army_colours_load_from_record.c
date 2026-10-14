#include "types.h"
#include "game/design_map.h"

extern u8 gSavedArmyColours[];

/* Copies the army colours of a design map record into gSavedArmyColours. */
void SavedArmyColours_LoadFromRecord(DesignMapRecord* record) {
  int i;

  for (i = 0; i < 5; i++) {
    gSavedArmyColours[i] = record->armyColours[i];
  }
}
