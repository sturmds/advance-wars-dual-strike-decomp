#include "types.h"

void* ScriptSlot_Start(const void* descriptor, int arg);
void Ov006_ClearLayer1Box(void);
void Ov006_ClearWindowsAndBlend(void);

extern u8 data_ov006_0236b908[];

void func_ov006_023578bc(void) {
  ScriptSlot_Start(data_ov006_0236b908, 0);
  Ov006_ClearLayer1Box();
  Ov006_ClearWindowsAndBlend();
}
