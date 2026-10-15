#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_LoadBmap0a0Palette(int armyIndex, int paletteSlot);

/* Loads /bmap/0a0 into palette slot 0 for a player: army index armyColour - 1,
 * 0 for the neutral slot. Gfx_LoadBmap0a0Palette ignores the index. */
void Gfx_LoadPlayerBmap0a0Palette(int player) {
  int armyIndex;

  if (player == 0) {
    armyIndex = 0;
  } else {
    armyIndex = MAP_STATE->players[player].armyColour - 1;
  }
  Gfx_LoadBmap0a0Palette(armyIndex, 0);
}
