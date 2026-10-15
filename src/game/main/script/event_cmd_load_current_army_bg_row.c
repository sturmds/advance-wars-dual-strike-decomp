#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"

extern Unk_0201d320 data_022977f4[];

void Palette_LoadPlayerArmyBgRow(int player);

/* Event script command: sets the palette slot (data_022977a4.unk_00) to 0, loads the current
 * player's army palette into the background palette row, then moves to the next record. */
BOOL EventCmd_LoadCurrentArmyBgRow(s16 index) {
  data_022977a4.unk_00 = 0;
  Palette_LoadPlayerArmyBgRow(MAP_STATE->currentPlayer);
  data_022977f4[index].unk_00++;
  return 1;
}
