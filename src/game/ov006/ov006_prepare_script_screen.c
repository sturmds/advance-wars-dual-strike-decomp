#include "types.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void Gfx_ClearLayerMap(int a);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void func_ov006_023505c4(void);

void Ov006_PrepareScriptScreen(void) {
  PopupMenu_PushState();
  func_0201e954();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  func_ov006_023505c4();
}
