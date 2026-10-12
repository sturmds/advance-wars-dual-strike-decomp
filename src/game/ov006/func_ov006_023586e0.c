#include "types.h"
#include "game/Unk_02088d04.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);
void* ScriptSlot_Start(const void* descriptor, int arg);
void Ov006_PostRequest(int request);

extern Unk_027c0280* data_027c0280;
extern u8 data_ov006_0236c088[];

/* Opens the request 6 screen: clears a 12x10 box at (2, 2) on layer 1, starts the
 * script data_ov006_0236c088 and posts request 6. */
void func_ov006_023586e0(void) {
  PopupMenu_PushState();
  func_0201e954();
  Bg_FillRect(data_027c0280->layer1Map, 2, 2, 12, 10, 0);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
  ScriptSlot_Start(data_ov006_0236c088, 0);
  Ov006_PostRequest(6);
}
