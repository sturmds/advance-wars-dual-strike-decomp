#include "types.h"

void Font_SetDigitEndTile(int arg);
void* ScriptSlot_StartGetIndex(const void* script, void* parent);
extern u8 data_ov006_0236b8e0[];

void func_ov006_02357770(void) {
  Font_SetDigitEndTile(220);
  ScriptSlot_StartGetIndex(data_ov006_0236b8e0, (void*)0);
}
