#include "types.h"
#include "game/Unk_022977a4.h"

void Gfx_LoadBmap0a0Palette(int armyIndex, int paletteSlot);

/* Loads the /bmap/0a0 palette into slot data_022977a4.unk_00 and records
 * player in unk_01. */
void EventScript_SetPlayerPalette(int player)
{
    Gfx_LoadBmap0a0Palette(player - 1, data_022977a4.unk_00);
    data_022977a4.unk_01 = player;
}
