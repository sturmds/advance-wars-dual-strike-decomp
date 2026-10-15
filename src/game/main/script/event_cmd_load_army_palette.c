#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/player.h"

extern Unk_0201d320 data_022977f4[];

extern u8 gEventSkipState;
void EventScript_SetArmyPalette(int army);

/* Event script command: loads the palette of a player's army through
 * EventScript_SetArmyPalette. The record value is a player slot, 5 meaning the current
 * player; slot 0 (neutral) loads the Black Hole palette. Does nothing when
 * gEventSkipState is 1. */
BOOL EventCmd_LoadArmyPalette(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];
  int player = script->unk_00->unk_08;
  int army;

  if (player == 5) {
    player = MAP_STATE->currentPlayer;
  }
  if (gEventSkipState != 1) {
    if (player == 0) {
      army = ARMY_BLACK_HOLE;
    } else {
      army = MAP_STATE->players[player].armyColour;
      /* needed to match: no effect on the value */
      if (army == ARMY_BLACK_HOLE) {
        army = ARMY_BLACK_HOLE;
      }
    }
    EventScript_SetArmyPalette(army);
  }
  script->unk_00++;
  return 1;
}
