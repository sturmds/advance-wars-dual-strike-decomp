#include "types.h"
#include "game/script_slot.h"

extern u8 gEventSkipState;
extern ScriptCommand data_021584f8[];
extern ScriptCommand data_02158520[];

void ScriptSlot_StopByScript(const void* script);

void func_020154c4(void) {
  if (gEventSkipState == 2) {
    gEventSkipState = 0;
  }
  ScriptSlot_StopByScript(data_021584f8);
  ScriptSlot_StopByScript(data_02158520);
}
