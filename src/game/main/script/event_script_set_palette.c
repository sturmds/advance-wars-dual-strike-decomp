#include "types.h"
#include "game/event_script.h"

extern u8 data_022900b8;
void EventScript_SetPlayerPalette(int player);

/* Script command: loads the palette for the player in unk_08. When data_022900b8 is 1
 * the command only advances. */
BOOL EventScript_SetPalette(s16 index)
{
    s16 player = data_022977f0[index].pc->unk_08;

    if (data_022900b8 != 1) {
        EventScript_SetPlayerPalette(player);
    }
    data_022977f0[index].pc++;
    return TRUE;
}
