#include "types.h"
#include "game/script_slot.h"

extern ScriptCommand data_0215837c[];
extern ScriptCommand data_02158394[];
extern ScriptCommand data_021583b4[];

void ScriptSlot_StopByScript(const ScriptCommand* script);

/* Stops the three text printing scripts (the ones Text_IsPrinting checks). */
void Text_StopPrinting(void) {
  ScriptSlot_StopByScript(data_0215837c);
  ScriptSlot_StopByScript(data_02158394);
  ScriptSlot_StopByScript(data_021583b4);
}
