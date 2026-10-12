#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotPlayer data_02297801[];

void EventHold_ReleaseOnPower(EventScriptSlot* slot);

/* Event script command: holds the script (hold = the check function, which
 * clears it when the condition is met) for the player slot in the record.
 * The check runs once at once, then the script moves to the next record and
 * the slot's run ends for this frame. */
BOOL EventCmd_HoldUntilPower(s16 index) {
  data_02297801[index].player = data_022977f4[index].unk_00->unk_08;
  data_022977f8[index].hold = (u32)EventHold_ReleaseOnPower;
  EventHold_ReleaseOnPower(&data_022977f0[index]);
  data_022977f4[index].unk_00++;
  return 0;
}
