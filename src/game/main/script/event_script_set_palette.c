#include "types.h"
#include "game/event_script.h"

extern u8 gEventSkipState;
void EventScript_SetPlayerPalette(int player);

/* Script command: loads the palette for the player in unk_08. When gEventSkipState is 1
 * the command only advances. */
BOOL EventScript_SetPalette(s16 index)
{
    s16 player = data_022977f0[index].pc->unk_08;

    if (gEventSkipState != 1) {
        EventScript_SetPlayerPalette(player);
    }
    data_022977f0[index].pc++;
    return TRUE;
}
