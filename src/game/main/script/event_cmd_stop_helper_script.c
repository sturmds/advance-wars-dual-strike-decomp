#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
extern const EventScriptCommand data_021584d8[];

s16 EventScript_StopAllWithScript(const EventScriptCommand* script);

/* Event script command: frees every slot running the script at data_021584d8
 * (started by EventCmd_StartHelperScript), then goes on. */
BOOL EventCmd_StopHelperScript(s16 index) {
  EventScript_StopAllWithScript(data_021584d8);
  data_022977f4[index].unk_00++;
  return TRUE;
}
