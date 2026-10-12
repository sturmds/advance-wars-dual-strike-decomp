#include "types.h"
#include "game/event_script.h"

void EventPicture_DrawAtStep(int step);

/* Hold function: counts the slot's counter down by 1 each frame, redrawing the picture
 * frame at that step (EventPicture_DrawAtStep). At 0 the picture is fully in and the hold clears. */
void EventHold_SlidePictureIn(EventScriptSlot* slot) {
  slot->counter--;
  EventPicture_DrawAtStep(slot->counter & 0xff);
  if (slot->counter == 0) {
    slot->counter = 0;
    slot->hold = 0;
  }
}
