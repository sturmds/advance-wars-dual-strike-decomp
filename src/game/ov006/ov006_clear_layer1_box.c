#include "types.h"
#include "game/Unk_02088d04.h"

void func_0201e954(void);
void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;

/* Clears the 18x8 tile box at (2, 5) on layer 1. */
void Ov006_ClearLayer1Box(void) {
  func_0201e954();
  Bg_FillRect(data_027c0280->layer1Map, 2, 5, 18, 8, 0);
  Gfx_RequestLayerCopy(1);
}
