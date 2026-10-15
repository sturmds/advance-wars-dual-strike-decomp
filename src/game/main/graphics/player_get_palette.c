#include "types.h"
#include "game/Unk_02080440.h"

extern u16 gArmyPalettes[][16];

/* Returns the 16-colour palette (32 bytes) of a player's army: row
 * armyColour - 1 of gArmyPalettes. */
const u16* Player_GetPalette(int player) {
  return gArmyPalettes[MAP_STATE->players[player].armyColour - 1];
}
