#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;

/* Clears the 14x24 tile block at the top left of layers 0 and 1 and queues both for copying. */
void Layers_ClearLeftBlock(void) {
  Bg_FillRect(data_027c0280->layer0Map, 0, 0, 14, 24, 0);
  Bg_FillRect(data_027c0280->layer1Map, 0, 0, 14, 24, 0);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
}
