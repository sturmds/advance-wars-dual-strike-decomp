#include "types.h"
#include "game/Unk_02088d04.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void Ov006_OpenPopupMenu(void);

extern Unk_027c0280* data_027c0280;

void func_ov006_023545ec(void) {
  PopupMenu_PushState();
  func_0201e954();
  Bg_FillRect(data_027c0280->layer1Map, 2, 2, 12, 10, 0);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  Ov006_OpenPopupMenu();
}
