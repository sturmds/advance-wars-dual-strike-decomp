#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"

extern Unk_0201d320 data_022977f4[];

void EventScript_SetArmyPalette(int army);

/* Event script command: like EventCmd_LoadCurrentArmyPalette but into palette slot 15
 * (data_022977a4.unk_00), through EventScript_SetArmyPalette. */
BOOL EventCmd_LoadCurrentArmyPaletteSlot15(s16 index) {
  data_022977a4.unk_00 = 15;
  EventScript_SetArmyPalette(
      MAP_STATE->players[MAP_STATE->currentPlayer].armyColour);
  data_022977f4[index].unk_00++;
  return 1;
}
