#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

extern u16 gArmyPalettes[][16];

/* Returns the 16-colour palette (32 bytes) of a player's army: row
 * armyColour - 1 of gArmyPalettes. */
const u16* Player_GetPalette(int player) {
  return gArmyPalettes[STATE->players[player].armyColour - 1];
}
