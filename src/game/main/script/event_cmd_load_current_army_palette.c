#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"

extern Unk_0201d320 data_022977f4[];

void Gfx_LoadPlayerBmap0a0Palette(int player);

/* Event script command: loads the current player's palette and records the
 * player's army colour in the script state (data_022977a4.armyColour), then
 * moves to the next record. */
BOOL EventCmd_LoadCurrentArmyPalette(s16 index) {
  Gfx_LoadPlayerBmap0a0Palette(MAP_STATE->currentPlayer);
  data_022977a4.armyColour =
      MAP_STATE->players[MAP_STATE->currentPlayer].armyColour;
  data_022977f4[index].unk_00++;
  return 1;
}
