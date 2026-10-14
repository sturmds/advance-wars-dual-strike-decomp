#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_ov006_0235f690.h"

void func_0201e954(void);
void Ov006_ClearLayer1Box(void);
void Ov006_ClearWindowsAndBlend(void);
void func_020c4018(void);
void Font_SetDigitEndTile(int tile);
void ScriptSlot_Start(const void* script, int arg);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern ScriptCommand data_ov006_0236b978[];

void func_ov006_02357eac(int id) {
  func_0201e954();
  Ov006_ClearLayer1Box();
  Ov006_ClearWindowsAndBlend();
  func_020c4018();
  Font_SetDigitEndTile(220);
  data_ov006_0236d640->chosenSlot = id;
  ScriptSlot_Start(data_ov006_0236b978, 0);
}
