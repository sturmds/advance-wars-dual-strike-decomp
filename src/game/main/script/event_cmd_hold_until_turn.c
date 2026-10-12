#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern EventSlotPlayer data_02297801[];
extern EventSlotDay data_02297802[];

void EventHold_ReleaseOnTurn(EventScriptSlot* slot);

/* Event script command: holds the script until the turn of the player slot
 * (record value 0x08) on the day (0x0a; negative: any day). The check runs
 * once at once, then the script moves to the next record and the slot's run
 * ends for this frame. */
BOOL EventCmd_HoldUntilTurn(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];

  data_02297801[index].player = script->unk_00->unk_08;
  data_02297802[index].day = script->unk_00->unk_0a;
  data_022977f8[index].hold = (u32)EventHold_ReleaseOnTurn;
  EventHold_ReleaseOnTurn(&data_022977f0[index]);
  script->unk_00++;
  return 0;
}
