#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Returns the entry at (x, y) of the 32-tile-wide tile map of layer 0-3,
 * or NULL for another layer. */
u16* Bg_GetMapEntry(int layer, int x, int y) {
  switch (layer) {
    case 0:
      return &data_027c0280->layer0Map[x + y * 32];
    case 1:
      return &data_027c0280->layer1Map[x + y * 32];
    case 2:
      return &data_027c0280->layer2Map[x + y * 32];
    case 3:
      return &data_027c0280->layer3Map[x + y * 32];
  }
  return NULL;
}
