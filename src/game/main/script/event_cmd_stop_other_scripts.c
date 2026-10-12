#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];

void EventScript_Stop(s16 slot);

/* Event script command: stops every other script slot, moves to the next
 * record and ends the slot's run for this frame. */
BOOL EventCmd_StopOtherScripts(s16 index) {
  u8 i;

  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++) {
    if (i != index) {
      EventScript_Stop(i);
    }
  }
  data_022977f4[index].unk_00++;
  return FALSE;
}
