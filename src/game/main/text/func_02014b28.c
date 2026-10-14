#include "types.h"
#include "game/event_script.h"
#include "game/script_slot.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
s16 EventScript_FindSlot(const EventScriptCommand* script);
int func_01ff9bb4(int arg);

extern TaskScriptCmd data_02158354[];
extern TaskScriptCmd data_02158344[];
extern EventScriptCommand data_02166a88[];
extern EventScriptCommand data_02166b08[];
extern EventScriptCommand data_02167880[];
extern ScriptCommand data_ov000_0234f2dc[];
extern u8 MainLoop_MapFrame[];

/* Checked by the text printer update before it acts on a touch press (button
 * bit 0x400). TRUE while the data_02158354 task runs; FALSE while the
 * data_02158344 task, any of the event scripts or the data_ov000_0234f2dc slot
 * script runs; otherwise whether the main loop is MainLoop_MapFrame. Purpose
 * guessed. */
BOOL func_02014b28(void) {
  if (Task_FindByScript(data_02158354) != NULL) {
    return TRUE;
  }
  if (Task_FindByScript(data_02158344) != NULL) {
    return FALSE;
  }
  if (EventScript_FindSlot(data_02166a88) != -1) {
    return FALSE;
  }
  if (EventScript_FindSlot(data_02166b08) != -1) {
    return FALSE;
  }
  if (EventScript_FindSlot(data_02167880) != -1) {
    return FALSE;
  }
  if (ScriptSlot_Find(data_ov000_0234f2dc) != -1) {
    return FALSE;
  }
  return func_01ff9bb4((int)MainLoop_MapFrame) != 0;
}
