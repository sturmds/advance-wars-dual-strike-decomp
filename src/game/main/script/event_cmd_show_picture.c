#include "types.h"
#include "game/Unk_0201d320.h"

extern u8 data_022900b8;
extern Unk_0201d320 data_022977f4[];

void EventScript_ShowPicture(s16 index);

/* Event script command: runs EventScript_ShowPicture for this slot (it loads a file and puts a tile
 * block on the screen; "picture" is a guess), unless data_022900b8 is 1. Then moves to the
 * next record. */
BOOL EventCmd_ShowPicture(s16 index)
{
    if (data_022900b8 != 1) {
        EventScript_ShowPicture(index);
    }
    data_022977f4[index].unk_00++;
    return 0;
}
