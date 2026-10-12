#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/event_script.h"

void EventPicture_DrawAtStep(int step);
void EventScript_LoadCoPicture(int coMood);
/* Hold function: counts the slot's counter up by 1 each frame, redrawing the picture
 * frame at that step (EventPicture_DrawAtStep). At step 15 it loads the CO picture for
 * data_022977a4.unk_02 and switches to EventHold_SlidePictureIn. */
void EventHold_SlidePictureIn(EventScriptSlot* slot);

/* Hold function: counts the slot's counter up by 1 each frame, redrawing the picture
 * frame at that step (EventPicture_DrawAtStep). At step 15 it loads the CO picture for
 * data_022977a4.unk_02 and switches to EventHold_SlidePictureIn. */
void EventHold_SlidePictureOut(EventScriptSlot* slot) {
  slot->counter++;
  EventPicture_DrawAtStep(slot->counter & 0xff);
  if (slot->counter == 15) {
    EventScript_LoadCoPicture(data_022977a4.unk_02);
    slot->hold = (u32)EventHold_SlidePictureIn;
  }
}
