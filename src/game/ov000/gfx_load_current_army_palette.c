#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022977a4.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void Gfx_LoadBmap0a0Palette(int armyIndex, int paletteSlot);

/* Loads the palette of the current player's army (Player.armyColour, the
 * palette) into palette slot 15 and records the slot in the script engine
 * state. Gfx_LoadBmap0a0Palette ignores its first argument. */
void Gfx_LoadCurrentArmyPalette(void) {
  data_022977a4.unk_00 = 15;
  Gfx_LoadBmap0a0Palette(STATE->players[STATE->currentPlayer].armyColour - 1,
                         data_022977a4.unk_00);
}
