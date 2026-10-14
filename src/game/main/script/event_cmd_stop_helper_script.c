#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_0201d320.h"
#include "game/event_script.h"

extern Unk_0201d320 data_022977f4[];
void ChoicePrompt_Init(void);
void TextBox_UpdateSkipInput(void);
void func_0201b888(void);
void ChoicePrompt_Update(void);
void EventScript_StopTextIfSkipping(void);

EventScriptCommand data_021584d8[] = {
#include "data/main/data_021584d8.inc.c"
};
ScriptCommand data_021584f8[] = {
#include "data/main/data_021584f8.inc.c"
};
ScriptCommand data_02158520[] = {
#include "data/main/data_02158520.inc.c"
};

s16 EventScript_StopAllWithScript(const EventScriptCommand* script);

/* Event script command: frees every slot running the script at data_021584d8
 * (started by EventCmd_StartHelperScript), then goes on. */
BOOL EventCmd_StopHelperScript(s16 index) {
  EventScript_StopAllWithScript(data_021584d8);
  data_022977f4[index].unk_00++;
  return TRUE;
}
