#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

#define STATE (data_027c027c->unk_48[data_027c0284])

void Gfx_LoadPlayerBmap0a0Palette(int player);

/* Event script command: loads the current player's palette and records the
 * player's army colour in the script state (data_022977a4.armyColour), then
 * moves to the next record. */
BOOL EventCmd_LoadCurrentArmyPalette(s16 index)
{
    Gfx_LoadPlayerBmap0a0Palette(STATE->currentPlayer);
    data_022977a4.armyColour = STATE->players[STATE->currentPlayer].armyColour;
    data_022977f4[index].unk_00++;
    return 1;
}
