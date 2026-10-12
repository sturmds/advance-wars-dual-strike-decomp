#include "types.h"
#include "game/event_script.h"

extern s16 data_027c0240;
extern s16 data_027c024c;
extern s16 data_02297780;

void CallbackList_Add(void* callback);
void* func_0201dc48(void);

/* Hold function of the screen slide-out: takes 4 off the slot's counter each frame
 * and sets both screen offsets (data_027c024c, data_027c0240) to -112 minus the counter.
 * Sets data_02297780 to the counter, and queues func_0201dc48 to copy it. At counter 0
 * or less it clears the hold. Counterpart of EventHold_StepScreenSlideIn. */
void EventHold_StepScreenSlideOut(EventScriptSlot* slot) {
  slot->counter -= 4;
  if (slot->counter <= 0) {
    slot->hold = 0;
    slot->counter = 0;
  }
  data_027c024c = -112 - slot->counter;
  data_027c0240 = -112 - slot->counter;
  data_02297780 = slot->counter;
  CallbackList_Add(func_0201dc48);
}
