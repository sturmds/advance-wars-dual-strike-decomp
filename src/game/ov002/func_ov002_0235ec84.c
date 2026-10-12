#include "types.h"
#include "game/script_slot.h"

extern ScriptCommand data_ov002_02369724[];
extern ScriptCommand data_ov002_02369744[];
extern ScriptCommand data_ov002_02369764[];
void Task_DestroyByScript(const ScriptCommand* script);

void func_ov002_0235ec84(void) {
  Task_DestroyByScript(data_ov002_02369724);
  Task_DestroyByScript(data_ov002_02369744);
  Task_DestroyByScript(data_ov002_02369764);
}
