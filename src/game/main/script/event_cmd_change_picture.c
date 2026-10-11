#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/event_script.h"

extern u8 gEventSkipState;
extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotCounter data_022977fe[];

void EventScript_LoadCoPicture(int coMood);
void EventHold_SlidePictureOut(EventScriptSlot *slot);

/* Event script command: shows the CO picture whose mood is in the record. When the low byte
 * matches the current picture (data_022977a4.unk_02) it only reloads it. Otherwise it starts
 * EventHold_SlidePictureOut from step 0. Does nothing and returns 1 when gEventSkipState is 1;
 * otherwise it ends the slot's run. The s16 return type is needed to match. */
s16 EventCmd_ChangePicture(s16 index)
{
    int result;

    if (gEventSkipState != 1) {
        s16 coMood = data_022977f4[index].unk_00->unk_08;

        if ((data_022977a4.unk_02 & 0xff) == (coMood & 0xff)) {
            data_022977a4.unk_02 = coMood;
            EventScript_LoadCoPicture(data_022977a4.unk_02);
        } else {
            data_022977fe[index].counter = 0;
            data_022977a4.unk_02 = coMood;
            data_022977f8[index].hold = (u32)EventHold_SlidePictureOut;
        }
        result = 0;
    } else {
        result = 1;
    }
    data_022977f4[index].unk_00++;
    return result;
}
