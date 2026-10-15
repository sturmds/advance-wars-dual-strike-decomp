#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"

extern u8 gSavedArmyColours[];

/* Copies the army colour of the five player slots into gSavedArmyColours
 * (SavedArmyColours_Reset sets slot i back to colour i). */
void Player_SaveArmyColours(void) {
  int i;

  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    gSavedArmyColours[i] = MAP_STATE->players[i].armyColour;
  }
}
