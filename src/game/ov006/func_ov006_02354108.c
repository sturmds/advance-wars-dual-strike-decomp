#include "types.h"
#include "game/menu_item.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void Gfx_ClearLayerMap(int arg);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Font_SetDigitEndTile(int tile);
void PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                      int selected, int center);
void Ov006_PostRequest(int request);

extern const MenuItem data_ov006_0236b56c[];

void func_ov006_02354108(void) {
  PopupMenu_PushState();
  func_0201e954();
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  Font_SetDigitEndTile(220);
  PopupMenu_Create(data_ov006_0236b56c, 2, 2, 0, 0, 0);
  Ov006_PostRequest(5);
}
