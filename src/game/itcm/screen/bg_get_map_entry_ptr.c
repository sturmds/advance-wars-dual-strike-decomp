#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Address of the tile map entry at column x, row y of a layer's map
 * (32 entries per row). A layer number above 3 gives layer 0's. */
u16* Bg_GetMapEntryPtr(int layer, int x, int y) {
  switch (layer) {
    case 0:
    default:
      return data_027c0280->layer0Map + y * 32 + x;
    case 1:
      return data_027c0280->layer1Map + y * 32 + x;
    case 2:
      return data_027c0280->layer2Map + y * 32 + x;
    case 3:
      return data_027c0280->layer3Map + y * 32 + x;
  }
}
