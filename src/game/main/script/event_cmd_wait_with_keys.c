#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern u8 gEventSkipState;
extern u16 gEventScriptKeys;
extern Unk_0201d320 data_022977f4[];
extern EventSlotWait data_022977fc[];

/* Event script command: waits the number of frames in the record's argument,
 * and sets gEventScriptKeys, the key state EventScript_UpdateAll feeds the
 * game, to the record's unk_08. When the event is being skipped
 * (gEventSkipState is 1) it neither waits nor sets the keys and the script goes
 * on at once. Returns 1 to go on, 0 to end the run for this frame. */
s16 EventCmd_WaitWithKeys(s16 index) {
  int result;

  if (gEventSkipState == 1) {
    result = 1;
  } else {
    const EventScriptCommand* entry = data_022977f4[index].unk_00;

    gEventScriptKeys = entry->unk_08;
    data_022977fc[index].wait = entry->arg;
    result = 0;
  }
  data_022977f4[index].unk_00++;
  return result;
}
