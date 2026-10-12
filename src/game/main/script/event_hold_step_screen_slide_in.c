#include "types.h"
#include "game/event_script.h"

extern s16 data_027c0240;
extern s16 data_027c024c;
extern s16 data_02297780;

void CallbackList_Add(void* callback);
void func_0200c108(void);
void func_0201c87c(EventScriptSlot* slot);
void* func_0201dc48(void);

/* Hold function of EventCmd_StartScreenSlideIn: adds 4 to the slot's counter each frame
 * and sets both screen offsets (data_027c024c, data_027c0240) to -112 minus the counter. Sets
 * data_02297780 to the counter, and queues func_0201dc48 to copy it. At counter 47 or more
 * it hands the hold to func_0201c87c instead. */
void EventHold_StepScreenSlideIn(EventScriptSlot* slot) {
  slot->counter += 4;
  if (slot->counter >= 47) {
    slot->hold = (u32)func_0201c87c;
    func_0200c108();
    return;
  }
  data_027c024c = -112 - slot->counter;
  data_027c0240 = -112 - slot->counter;
  data_02297780 = slot->counter;
  CallbackList_Add(func_0201dc48);
}
