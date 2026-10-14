#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

extern u8 gSavedArmyColours[];

/* Copies the army colour of the five player slots into gSavedArmyColours
 * (SavedArmyColours_Reset sets slot i back to colour i). */
void Player_SaveArmyColours(void) {
  int i;

  for (i = 0; i < 5; i++) {
    gSavedArmyColours[i] = STATE->players[i].armyColour;
  }
}
