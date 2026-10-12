#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern const EventScriptCommand data_021584d8[];
extern u8 gEventSkipState;

/* Event script command: starts the script at data_021584d8 in a free slot and
 * stores the current record in the new slot's unk_14, unless the skip flag
 * gEventSkipState is 1. Then goes on. The script's purpose is unknown. */
BOOL EventCmd_StartHelperScript(s16 index) {
  EventScriptSlot* started;

  if (gEventSkipState != 1) {
    started = EventScript_Start(data_021584d8);
    started->unk_14 = (const EventScriptCommand*)data_022977f4[index].unk_00;
  }
  data_022977f4[index].unk_00++;
  return TRUE;
}
