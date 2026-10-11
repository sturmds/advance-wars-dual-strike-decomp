#include "types.h"
#include "game/event_script.h"

void func_0201cbbc(int step);

/* Hold function: counts the slot's counter down by 1 each frame, redrawing the picture
 * frame at that step (func_0201cbbc). At 0 the picture is fully in and the hold clears. */
void EventHold_SlidePictureIn(EventScriptSlot *slot)
{
    slot->counter--;
    func_0201cbbc(slot->counter & 0xff);
    if (slot->counter == 0) {
        slot->counter = 0;
        slot->hold = 0;
    }
}
