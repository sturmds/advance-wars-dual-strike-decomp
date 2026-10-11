#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"

extern u8 gEventSkipState;
extern Unk_0201d320 data_022977f4[];

void EventScript_LoadCoPicture(int coMood);

/* Event script command: shows the CO picture whose mood is in the record at once, without
 * the slide of EventCmd_ChangePicture, and remembers it in data_022977a4.unk_02. Does
 * nothing when gEventSkipState is 1. */
BOOL EventCmd_SetPicture(s16 index)
{
    if (gEventSkipState != 1) {
        data_022977a4.unk_02 = data_022977f4[index].unk_00->unk_08;
        EventScript_LoadCoPicture(data_022977a4.unk_02);
    }
    data_022977f4[index].unk_00++;
    return 0;
}
