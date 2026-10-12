#include "types.h"
#include "game/event_script.h"

int Task_FindByScript(void*);
extern u8 data_02158548[];

/* Hold function set by EventCmd_StartTextMapTask: clears the slot's hold
 * once the task it started no longer exists. */
void EventHold_ReleaseOnTextMapTaskEnd(EventScriptSlot* slot) {
  if (Task_FindByScript(data_02158548) == 0) {
    slot->hold = 0;
  }
}
