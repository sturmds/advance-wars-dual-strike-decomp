#include "types.h"
#include "game/Unk_02088d04.h"

void func_0201e954(void);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);
void func_020c4018(void);

/* Clears the 12x10 tile box at (2, 2) on layer 1. */
void Ov006_ClearLayer1MenuBox(void) {
  func_0201e954();
  Bg_FillRect(data_027c0280->layer1Map, 2, 2, 12, 10, 0);
  Gfx_RequestLayerCopy(1);
  func_020c4018();
}
