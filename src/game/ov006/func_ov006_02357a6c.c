#include "types.h"
#include "game/script_slot.h"

void* ScriptSlot_Start(const void* descriptor, int arg);
void Ov006_ClearLayer1Box(void);
void Ov006_ClearWindowsAndBlend(void);
extern ScriptCommand data_ov006_0236b940[];

void func_ov006_02357a6c(void) {
  ScriptSlot_Start(data_ov006_0236b940, 0);
  Ov006_ClearLayer1Box();
  Ov006_ClearWindowsAndBlend();
}
